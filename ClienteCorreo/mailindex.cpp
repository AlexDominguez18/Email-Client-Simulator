#include "mailindex.h"

#include <QDebug>

using namespace std::chrono;

MailIndex::MailIndex()
{
    //For a no paginated Index change the next set to 'false'
    index.isPaginated = true;
    piFile.open(PRIMARY_INDEX_FILE, std::ios::in | std::ios::out | std::ios::binary | std::ios::app);
    piFile.close();
    piFile.open(PRIMARY_INDEX_FILE,std::ios::in | std::ios::binary);
    if (piFile.is_open()){
        PrimaryIndex pi;
        piFile.seekg(0,std::ios_base::beg);
        piFile.read((char*)&pi,sizeof(PrimaryIndex));
        piFile.close();
        if (strcmp(pi.getKey(),"1") == 0 and pi.getValue() == 1){
            loadFilePi();
        }
        else{
            loadFileMails();
        }
    }
}

MailIndex::~MailIndex()
{
    if (!index.isPaginated){
        savePrimaryIndex();
    }
    else{
        primaryIndexUpdate(true);
    }
}

void MailIndex::insertPrimaryIndex(PrimaryIndex &pi)
{
    if (!index.isPaginated){
        index.insert(pi);
    }
    else{
        if (index.nodes < MAX_NODES){
            index.insert(pi);
            index.getLeastRecentlyUsed()->setData(pi);
            index.find(pi)->timestamp = getCurrentTimeMilliseconds();
        }
        std::fstream tmp;
        PrimaryIndex ip;
        unsigned int i;
        index.getLeastRecentlyUsed()->setData(pi);
        index.find(pi)->timestamp = getCurrentTimeMilliseconds();
        piFile.open(PRIMARY_INDEX_FILE,std::ios::in | std::ios::out | std::ios::binary);
        tmp.open(TMP_PRIMARY_INDEX,std::ios::out | std::ios::binary);
        i = 0;
        do{
            piFile.seekg(i*sizeof(PrimaryIndex),std::ios_base::beg);
            piFile.read((char*)&ip,sizeof(PrimaryIndex));
            tmp.write((char*)&ip,sizeof(PrimaryIndex));
            i++;
        }while(ip < pi);
        //Adding the new
        tmp.write((char*)&pi,sizeof(PrimaryIndex));
        for (;i < getPiCount();++i){
            piFile.seekg(i*sizeof(PrimaryIndex),std::ios_base::beg);
            piFile.read((char*)&ip,sizeof(PrimaryIndex));
            tmp.write((char*)&ip,sizeof(PrimaryIndex));
        }
        piFile.close();
        tmp.close();
        remove(PRIMARY_INDEX_FILE);
        rename(TMP_PRIMARY_INDEX,PRIMARY_INDEX_FILE);
        primaryIndexUpdate(true);
    }
}

void MailIndex::deletePrimaryIndex(PrimaryIndex &pi)
{
    AVLNode<PrimaryIndex>*& d = index.find(pi);
    if (!index.isPaginated){
        index.deleteData(d);
    }
    else{
        if (d){
            index.deleteData(d);
        }
        PrimaryIndex ip;
        std::fstream tmp;
        unsigned int fileLenght;
        piFile.open(PRIMARY_INDEX_FILE,std::ios::in | std::ios::out | std::ios::binary);
        tmp.open(TMP_PRIMARY_INDEX,std::ios::out | std::ios::binary);
        fileLenght = getPiCount();
        if (tmp.is_open()){
            piFile.seekg(0,std::ios_base::beg);
            for (unsigned int i(0),j(0); i < fileLenght;++i){
                piFile.seekg(i*sizeof(PrimaryIndex),std::ios::beg);
                piFile.read((char*)&ip,sizeof(PrimaryIndex));
                if (!(ip == pi)){
                    tmp.seekp(j*sizeof(PrimaryIndex),std::ios::beg);
                    tmp.write((char*)&ip,sizeof(PrimaryIndex));
                    ++j;
                }

            }
            tmp.close();
            piFile.close();
            remove(PRIMARY_INDEX_FILE);
            rename(TMP_PRIMARY_INDEX,PRIMARY_INDEX_FILE);
            primaryIndexUpdate(true);
        }
    }
}

void MailIndex::savePrimaryIndex()
{
    unsigned int i = 1;
    PrimaryIndex pi;
    piFile.open(PRIMARY_INDEX_FILE,std::ios::in | std::ios::out | std::ios::binary);
    if (piFile.is_open()){
        piFile.seekp(i*sizeof(PrimaryIndex),std::ios_base::beg);
        index.saveTree(piFile);
        piFile.close();
        primaryIndexUpdate(true);
    }
}

void MailIndex::primaryIndexUpdate(bool updated)
{
    PrimaryIndex pi;
    piFile.open(PRIMARY_INDEX_FILE,std::ios::in | std::ios::out | std::ios::binary);
    if (piFile.is_open()){
        piFile.seekp(0,std::ios_base::beg);
        if (updated){
            pi.setKey("1");
            pi.setValue(1);
            piFile.write((char*)&pi,sizeof(PrimaryIndex));
        }
        else{
            piFile.write((char*)&pi,sizeof(pi));
        }
        piFile.close();
    }
}

void MailIndex::loadFileMails()
{
    unsigned int pos,fileLenght = mailDao.getFileLength();
    Mail* m = new Mail();
    PrimaryIndex ip;

    if (index.isPaginated){
        fileLenght = MAX_NODES;
    }
    for (unsigned int i = 0; i < fileLenght; i++){
        m = mailDao.getById(i+1);
        if (m != nullptr){
            pos = i * sizeof(Mail);
            ip.setValue(pos);
            ip.setKey(std::to_string(m->getId()).c_str());
            index.insert(ip);
        }
    }
}

void MailIndex::loadFileMailsSi()
{
    unsigned int mailsCount = mailDao.getFileLength();
    Mail* m = new Mail();
    indexSender = new AVLTree<SecondaryIndex>();
    indexSender->isPaginated = false;
    indexAddressee = new AVLTree<SecondaryIndex>();
    indexAddressee->isPaginated = false;
    SecondaryIndex siSender;
    SecondaryIndex siAddressee;

    for (unsigned int i = 1; i < mailsCount; i++){
        m = mailDao.getById(i);
        if (m != nullptr){
            //Secondary Index Sender
            if (indexSender->find(SecondaryIndex(m->getSender()))){
                indexSender->retrieve(indexSender->find(SecondaryIndex(m->getSender()))).insertValue(m->getId());
            }
            else{
                indexSender->insert(SecondaryIndex(m->getSender()));
                indexSender->retrieve(indexSender->find(SecondaryIndex(m->getSender()))).insertValue(m->getId());
            }
            //Secondary Index Addressee
            if (indexAddressee->find(SecondaryIndex(m->getAddressee()))){
                indexAddressee->retrieve(indexAddressee->find(SecondaryIndex(m->getAddressee()))).insertValue(m->getId());
            }
            else{
                indexAddressee->insert(SecondaryIndex(m->getAddressee()));
                indexAddressee->retrieve(indexAddressee->find(SecondaryIndex(m->getAddressee()))).insertValue(m->getId());
            }
        }
    }
}

void MailIndex::deleteSecondaryIndex()
{
    delete indexSender;
    delete indexAddressee;
}

void MailIndex::loadFilePi()
{
    unsigned int i, indexCount;
    PrimaryIndex pi;
    piFile.open(PRIMARY_INDEX_FILE,std::ios::in | std::ios::binary);
    indexCount = getPiCount();
    if (index.isPaginated){
        indexCount = MAX_NODES;
    }
    if (piFile.is_open()){
        piFile.seekg(0,std::ios_base::beg);
        piFile.read((char*)&pi,sizeof(PrimaryIndex));
        i = 1;
        while (i < indexCount) {
            piFile.seekg(i*sizeof(PrimaryIndex),std::ios_base::beg);
            piFile.read((char*)&pi,sizeof(PrimaryIndex));
            index.insert(pi);
            i++;
        }
        piFile.close();
    }
}

Mail *MailIndex::findMail(const char *key)
{
    Mail* m = new Mail();
    PrimaryIndex ip;
    AVLNode<PrimaryIndex>* indexSearched;

    if (std::atol(key) <= mailDao.getFileLength()){
        indexSearched = index.find(PrimaryIndex(key));
        if (indexSearched){
            ip = index.retrieve(index.find(PrimaryIndex(key)));
            m = mailDao.getByPos(ip.getValue());
            if (index.isPaginated){
                index.find(PrimaryIndex(key))->timestamp = getCurrentTimeMilliseconds();
            }
            return m;
        }
        else{//Hit page
            piFile.open(PRIMARY_INDEX_FILE,std::ios::in | std::ios::binary);
            if (piFile.is_open()){
                unsigned int indexCount = getPiCount();
                for (unsigned int i(1); i < indexCount; ++i){
                    piFile.seekg(i*sizeof(PrimaryIndex),std::ios_base::beg);
                    piFile.read((char*)&ip,sizeof(PrimaryIndex));
                    if (std::strcmp(ip.getKey(),key) == 0){
                        m = mailDao.getByPos(ip.getValue());
                        index.getLeastRecentlyUsed()->setData(ip);
                        index.find(ip)->timestamp = getCurrentTimeMilliseconds();
                        piFile.close();
                        return m;
                    }
                }
                m = mailDao.getById(atol(key));
                piFile.close();
                return m;
            }
        }
    }
    return nullptr;
}

List<Mail> &MailIndex::getMailsSender(const char *key)
{
    Mail m;
    PrimaryIndex pi;
    mails.deleteAll();
    if (indexSender->find(SecondaryIndex(key))){
        List<unsigned int>::Node* aux(indexSender->retrieve(indexSender->find(SecondaryIndex(key))).getValues().getFirstPos());
        while (aux != nullptr){
            if (index.find(PrimaryIndex(std::to_string(aux->getData()).c_str()))){
                pi = index.retrieve(index.find(PrimaryIndex(std::to_string(aux->getData()).c_str())));
                m = *mailDao.getByPos(pi.getValue());
            }
            else{
                m = *mailDao.getById(aux->getData());
            }
            mails.insertData(mails.getLastPos(),m);
            aux = aux->getNext();
        }
    }
    return mails;
}

List<Mail> &MailIndex::getMailsAddressee(const char *key)
{
    Mail m;
    PrimaryIndex pi;
    mails.deleteAll();
    if (indexAddressee->find(SecondaryIndex(key))){
        List<unsigned int>::Node* aux(indexAddressee->retrieve(indexAddressee->find(SecondaryIndex(key))).getValues().getFirstPos());
        while (aux != nullptr){
            if (index.find(PrimaryIndex(std::to_string(aux->getData()).c_str()))){
                pi = index.retrieve(index.find(PrimaryIndex(std::to_string(aux->getData()).c_str())));
                m = *mailDao.getByPos(pi.getValue());
            }
            else{
                m = *mailDao.getById(aux->getData());
            }
            mails.insertData(mails.getLastPos(),m);
            aux = aux->getNext();
        }
    }
    return mails;
}

List<Mail> &MailIndex::getMailsBoolean(std::string text)
{
    int pos;
    Mail m;
    PrimaryIndex pi;
    std::string sender,addressee;

    List<unsigned int>::Node* firstS = nullptr;
    List<unsigned int>::Node* firstA = nullptr;
    List<unsigned int>::Node* s;
    List<unsigned int>::Node* a;

    mails.deleteAll();
    if (text.find(AND_BOOLEAN) != std::string::npos){
        pos = text.find(AND_BOOLEAN);
        sender = text.substr(0,pos-1);
        addressee = text.substr(pos+4,text.length());
        if (indexSender->find(SecondaryIndex(sender.c_str()))){
            firstS = (indexSender->retrieve(indexSender->find(SecondaryIndex(sender.c_str()))).getValues().getFirstPos());
        }
        if (indexAddressee->find(SecondaryIndex(addressee.c_str()))){
            firstA = (indexAddressee->retrieve(indexAddressee->find(SecondaryIndex(addressee.c_str()))).getValues().getFirstPos());
        }
        s = firstS;
        while (s != nullptr){
            a = firstA;
            while (a != nullptr){
                if (s->getData() == a->getData()){
                    if (index.find(PrimaryIndex(std::to_string(s->getData()).c_str()))){
                        pi = index.retrieve(index.find(PrimaryIndex(std::to_string(s->getData()).c_str())));
                        m = *mailDao.getByPos(pi.getValue());
                    }
                    else{
                        m = *mailDao.getById(s->getData());
                    }
                    mails.insertData(mails.getLastPos(),m);
                }
                a = a->getNext();
            }
            s = s->getNext();
        }
    }
    else if (text.find(OR_BOOLEAN) != std::string::npos){
        pos = text.find(OR_BOOLEAN);
        sender = text.substr(0,pos-1);
        addressee = text.substr(pos+3,text.length());
        if (indexSender->find(SecondaryIndex(sender.c_str()))){
            firstS = (indexSender->retrieve(indexSender->find(SecondaryIndex(sender.c_str()))).getValues().getFirstPos());
        }
        if (indexAddressee->find(SecondaryIndex(addressee.c_str()))){
            firstA = (indexAddressee->retrieve(indexAddressee->find(SecondaryIndex(addressee.c_str()))).getValues().getFirstPos());
        }
        s = firstS;
        while (s != nullptr){
            if (index.find(PrimaryIndex(std::to_string(s->getData()).c_str()))){
                pi = index.retrieve(index.find(PrimaryIndex(std::to_string(s->getData()).c_str())));
                m = *mailDao.getByPos(pi.getValue());
            }
            else{
                m = *mailDao.getById(s->getData());
            }
            mails.insertData(mails.getLastPos(),m);
            s = s->getNext();
        }
        a = firstA;
        while (a != nullptr){
            bool containsElement = false;
            s = firstS;
            while (s != nullptr){
                if (a->getData() == s->getData()){
                    containsElement = true;
                }
                s = s->getNext();
            }
            if (!containsElement){
                if (index.find(PrimaryIndex(std::to_string(a->getData()).c_str()))){
                    pi = index.retrieve(index.find(PrimaryIndex(std::to_string(a->getData()).c_str())));
                    m = *mailDao.getByPos(pi.getValue());
                }
                else{
                    m = *mailDao.getById(a->getData());
                }
                mails.insertData(mails.getLastPos(),m);
            }
            a = a->getNext();
        }
    }
    else{
        //Case of an not valid search
        return mails;
    }
    return mails;
}

unsigned int MailIndex::getPiCount()
{
   long long fileLength;
   if (piFile.is_open()){
        piFile.seekp(0,std::ios_base::end);
        fileLength = piFile.tellg();
        if (fileLength != -1){
            return fileLength/sizeof(PrimaryIndex);
        }
    }
   return 0;
}

long long MailIndex::getCurrentTimeMilliseconds()
{
    return duration_cast<std::chrono::milliseconds>
            (std::chrono::system_clock::now().time_since_epoch()).count();
}

void MailIndex::saveHashTable()
{
    qDebug() << "Saving Hash Table...";
    hashMails->save();
}

void MailIndex::loadHashTable(int loadMode)
{
    hashMails = new HashTable();
    if (loadMode != 0){
        qDebug() << "Reading HashFile";
        hashMails->load();
    }
    else{
        //Reading Mails File
        for (unsigned int i = 0; i < mailDao.getFileLength()-1; i++){
            Mail mail;
            HashTable::Index indexMail;
            mail = *mailDao.getById(i+1);
            strcpy(indexMail.key,mail.getSender());
            indexMail.value = i*sizeof(Mail);
            qDebug() << "i = " << i;
            hashMails->add(indexMail);
        }
    }
}

List<Mail> &MailIndex::findHashMail(char *key)
{
    List<HashTable::Index>& mailsIndex = hashMails->getIndexAt(key);
    List<HashTable::Index>::Node* aux(mailsIndex.getFirstPos());

    mails.deleteAll();
    while(aux != nullptr){
        if (std::strcmp(aux->getData().key,key) == 0){
            Mail mail;
            mail = *mailDao.getByPos(aux->getData().value);
            mails.insertData(mails.getLastPos(),mail);
        }
        aux = aux->getNext();
    }

    return mails;
}

void MailIndex::deleteHashTable()
{
    qDebug() << "Deleting Hash Table...";
    delete hashMails;
}
