#include "maildao.h"

#include <QDebug>

using namespace std;

MailDao::MailDao()
{
    file.open(MAIL_FILE,ios::in | ios::out | ios::binary| ios::app);
    file.close();
    file.open(MAIL_FILE,ios::in | ios::out | ios::binary);
}

MailDao::~MailDao()
{
    //This destructor close the file.
}

bool MailDao::add(Mail *m)
{
    QMessageBox message;
    unsigned int nBytes,idReaded;
    nBytes = m->getId();
    nBytes--;
    if (nBytes < getFileLength()){
        file.seekg(nBytes*sizeof(Mail),ios_base::beg);
        file.read((char*)&idReaded,sizeof(unsigned int));
        nBytes++;
        if (nBytes != idReaded){
            nBytes--;
            file.seekp(nBytes*sizeof(Mail),ios_base::beg);
            file.write((char*)m,sizeof(Mail));
            file.close();
            return true;
        }
        return false;
    }
    else{
        file.seekp(nBytes*sizeof(Mail),ios_base::beg);
        file.write((char*)m,sizeof(Mail));
        qDebug() << nBytes;
        return true;
    }
}

void MailDao::update(Mail *m, unsigned int p)
{
    p--;
    qDebug() << m->getAffair();
    file.seekp(p*sizeof(Mail),ios_base::beg);
    file.write((char*)m,sizeof(Mail));
}

Mail *MailDao::getByPos(unsigned int p)
{
    Mail* mail = new Mail();
    file.seekg(p,ios::beg);
    file.read((char*)mail,sizeof(Mail));
    if (mail->getId() == 0){
        return nullptr;
    }
    return mail;
}

Mail* MailDao::getById(unsigned int p)
{
    unsigned int j = getFileLength();
    Mail* mail = new Mail();
    p--;
    if (p < j){
        file.seekg(p*sizeof(Mail),ios_base::beg);
        file.read((char*)mail,sizeof(Mail));
        if (mail->getId() == 0){
            return nullptr;
        }
        return mail;
    }
    else{
        return nullptr;
    }
}

Mail* MailDao::getBySender(unsigned int p, const char *senderSearched)
{
    Mail* mail = new Mail();
    unsigned int id = p;
    p--;
    for (unsigned int i = 0;i <= p;i++){
        qDebug() << i;
        file.seekg(i*sizeof(Mail),ios::beg);
        file.read((char*)mail,sizeof(Mail));
        if (std::strcmp(mail->getSender(),senderSearched) == 0 and mail->getId() == id){
            return mail;
        }
    }
    return nullptr;
}

void MailDao::getBySender(const char *senderSearched,List<Mail>& mails)
{
    unsigned int i = 0,j;
    Mail mail;
    j = getFileLength();
    while(i < j){
        file.seekg(i*sizeof(Mail),ios_base::beg);
        file.read((char*)&mail,sizeof(Mail));
        if (std::strcmp(senderSearched,mail.getSender()) == 0){
            mails.insertData(mails.getLastPos(),mail);
        }
        i++;
    }
}

void MailDao::deleteBySender(unsigned int p,const char *senderSearched)
{
    Mail mail;
    Mail* nullMail = new Mail();
    unsigned int id = p;
    p--;
    for (unsigned int i = 0;i <= p;i++){
        file.seekg(i*sizeof(Mail),ios_base::beg);
        file.read((char*)&mail,sizeof(Mail));
        if (std::strcmp(mail.getSender(),senderSearched) == 0 and mail.getId() == id){
            file.seekp(i*sizeof(Mail),ios_base::beg);
            file.write((char*)nullMail,sizeof(Mail));
            break;
        }
    }
}

void MailDao::deleteById(unsigned int p)
{
    Mail* mail = new Mail();
    p--;
    file.seekp(p*sizeof(Mail),ios_base::beg);
    file.write((char*)mail,sizeof(Mail));
}

unsigned int MailDao::getFileLength()
{
    long long fileLength;
    file.seekp(0,ios_base::end);
    fileLength = file.tellg();
    if ((long long)fileLength != -1){ //File is good
        return fileLength/sizeof(Mail);
    }
    else{//File is not good
        file.close();
        file.open(MAIL_FILE,ios::in | ios::out | ios::binary);
        file.seekp(0,ios_base::end);
        fileLength = file.tellg();
        return fileLength/sizeof(Mail);
    }
    return 0;
}

bool MailDao::haveData()
{
    if (getFileLength() > 0){
        return true;
    }
    return false;
}
