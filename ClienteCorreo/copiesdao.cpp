#include "copiesdao.h"

#include <QDebug>

CopiesDAO::CopiesDAO()
{
    file.open(COPIES_FILE,std::ios::in | std::ios::out | std::ios::binary | std::ios::app);
    file.close();
}

CopiesDAO::~CopiesDAO(){ }

bool CopiesDAO::haveData()
{

    file.open(COPIES_FILE,std::ios::in | std::ios::out | std::ios::binary);
    file.seekg(0,std::ios::end);
    if (file.tellg() != -1){
        file.close();
        return true;
    }
    file.close();
    return false;
}

void CopiesDAO::openFile()
{
    file.open(COPIES_FILE,std::ios::in | std::ios::out | std::ios::binary);
}

void CopiesDAO::closeFile()
{
    file.close();
}

void CopiesDAO::write(Mail *mail)
{
    unsigned int id;
    int dateSize,timeSize,senderSize,addresseeSize,ccSize,ccoSize,affairSize,contentSize;
    //Size of each field
    dateSize = std::strlen(mail->getDate());
    timeSize = std::strlen(mail->getTime());
    senderSize = std::strlen(mail->getSender());
    addresseeSize = std::strlen(mail->getAddressee());
    ccSize = std::strlen(mail->getCc());
    ccoSize = std::strlen(mail->getCco());
    affairSize = std::strlen(mail->getAffair());
    contentSize = std::strlen(mail->getContent());
    //Writen the file
    id = mail->getId();
    file.write((char*)&id,sizeof(id));
    file.write((char*)&dateSize,sizeof(dateSize));
    file.write((char*)mail->getDate(),dateSize);
    file.write((char*)&timeSize,sizeof(timeSize));
    file.write((char*)mail->getTime(),timeSize);
    file.write((char*)&senderSize,sizeof(senderSize));
    file.write((char*)mail->getSender(),senderSize);
    file.write((char*)&addresseeSize,sizeof(addresseeSize));
    file.write((char*)mail->getAddressee(),addresseeSize);
    file.write((char*)&ccSize,sizeof(ccSize));
    file.write((char*)mail->getCc(),ccSize);
    file.write((char*)&ccoSize,sizeof(ccoSize));
    file.write((char*)mail->getCco(),ccoSize);
    file.write((char*)&affairSize,sizeof(affairSize));
    file.write((char*)mail->getAffair(),affairSize);
    file.write((char*)&contentSize,sizeof(contentSize));
    file.write((char*)mail->getContent(),contentSize);
}

void CopiesDAO::write(Mail *mail, std::fstream &tmpFile)
{
    unsigned int id;
    int dateSize,timeSize,senderSize,addresseeSize,ccSize,ccoSize,affairSize,contentSize;
    //Size of each field
    dateSize = std::strlen(mail->getDate());
    timeSize = std::strlen(mail->getTime());
    senderSize = std::strlen(mail->getSender());
    addresseeSize = std::strlen(mail->getAddressee());
    ccSize = std::strlen(mail->getCc());
    ccoSize = std::strlen(mail->getCco());
    affairSize = std::strlen(mail->getAffair());
    contentSize = std::strlen(mail->getContent());
    //Writen the file
    id = mail->getId();
    tmpFile.write((char*)&id,sizeof(id));
    tmpFile.write((char*)&dateSize,sizeof(dateSize));
    tmpFile.write((char*)mail->getDate(),dateSize);
    tmpFile.write((char*)&timeSize,sizeof(timeSize));
    tmpFile.write((char*)mail->getTime(),timeSize);
    tmpFile.write((char*)&senderSize,sizeof(senderSize));
    tmpFile.write((char*)mail->getSender(),senderSize);
    tmpFile.write((char*)&addresseeSize,sizeof(addresseeSize));
    tmpFile.write((char*)mail->getAddressee(),addresseeSize);
    tmpFile.write((char*)&ccSize,sizeof(ccSize));
    tmpFile.write((char*)mail->getCc(),ccSize);
    tmpFile.write((char*)&ccoSize,sizeof(ccoSize));
    tmpFile.write((char*)mail->getCco(),ccoSize);
    tmpFile.write((char*)&affairSize,sizeof(affairSize));
    tmpFile.write((char*)mail->getAffair(),affairSize);
    tmpFile.write((char*)&contentSize,sizeof(contentSize));
    tmpFile.write((char*)mail->getContent(),contentSize);
}

Mail *CopiesDAO::read()
{
    Mail* mail = new Mail();
    unsigned int id;
    int dateSize,timeSize,senderSize,addresseeSize,ccSize,ccoSize,affairSize,contentSize;
    char date[DATE_SIZE+1];
    char time[TIME_SIZE+1];
    char sender[MAIL_SIZE+1];
    char addressee[MAIL_SIZE+1];
    char cc[MAIL_SIZE+1];
    char cco[MAIL_SIZE+1];
    char affair[AFFAIR_SIZE+1];
    char content[CONTENT_SIZE+1];
    //Reading a Mail
    file.read((char*)&id,sizeof(id));
    mail->setId(id);
    file.read((char*)&dateSize,sizeof(dateSize));
    file.read(date,dateSize);
    date[dateSize] = NULL_CHAR;
    mail->setDate(date);
    file.read((char*)&timeSize,sizeof(timeSize));
    file.read(time,timeSize);
    time[timeSize] = NULL_CHAR;
    mail->setTime(time);
    file.read((char*)&senderSize,sizeof(senderSize));
    file.read(sender,senderSize);
    sender[senderSize] = NULL_CHAR;
    mail->setSender(sender);
    file.read((char*)&addresseeSize,sizeof(addresseeSize));
    file.read(addressee,addresseeSize);
    addressee[addresseeSize] = NULL_CHAR;
    mail->setAddressee(addressee);
    file.read((char*)&ccSize,sizeof(ccSize));
    file.read(cc,ccSize);
    cc[ccSize] = NULL_CHAR;
    mail->setCc(cc);
    file.read((char*)&ccoSize,sizeof(ccoSize));
    file.read(cco,ccoSize);
    cco[ccoSize] = NULL_CHAR;
    mail->setCco(cco);
    file.read((char*)&affairSize,sizeof(affairSize));
    file.read(affair,affairSize);
    affair[affairSize] = NULL_CHAR;
    mail->setAffair(affair);
    file.read((char*)&contentSize,sizeof(contentSize));
    file.read(content,contentSize);
    content[contentSize] = NULL_CHAR;
    mail->setContent(content);
    return mail;
}

Mail *CopiesDAO::getById(unsigned int id)
{
    Mail* mail;
    unsigned int fileLength = getFileLength();
    file.seekg(0,std::ios::beg);
    while (file.tellg() < fileLength){
        mail = read();
        if (mail->getId() == id){
            file.close();
            return mail;
        }
    }
    return nullptr;
}

void CopiesDAO::update(Mail *mail)
{
    Mail* m = new Mail();
    std::fstream tmp;
    unsigned int fileLenght = getFileLength();

    tmp.open(TMP_COPIES,std::ios::out | std::ios::binary);
    if (tmp.is_open()){
        file.seekg(0,std::ios::beg);
        while (file.tellg() < fileLenght){
            m = read();
            if (mail->getId() != m->getId()){
                write(m,tmp);
            }
            else{
                write(mail,tmp);
            }
        }
        file.close();
        tmp.close();
        remove(COPIES_FILE);
        rename(TMP_COPIES,COPIES_FILE);
    }
    else{
        qDebug() << "No se abrio el archivo tmp.";
    }

}

void CopiesDAO::deleteCopy(unsigned int id)
{
    std::fstream tmp;
    Mail* mail = new Mail();
    unsigned int fileLength = getFileLength();

    tmp.open(TMP_COPIES,std::ios::out | std::ios::binary);
    if (tmp.is_open()){
        file.seekg(0,std::ios::beg);
        while (file.tellg() < fileLength){
            mail = read();
            if (mail->getId() != id){
                write(mail,tmp);
            }
        }
        file.close();
        tmp.close();
        remove(COPIES_FILE);
        rename(TMP_COPIES,COPIES_FILE);
    }
    else{
        qDebug() << "No se pudo abrir el archivo temporal.";
    }

}

unsigned int CopiesDAO::getFileLength()
{
    file.seekg(0,std::ios::end);
    return file.tellg();
}

unsigned int CopiesDAO::getRecordsCounter()
{
    Mail* mail = new Mail();
    unsigned int i = 0,fileLength = getFileLength();
    file.seekg(0,std::ios::beg);
    while (file.tellg() < fileLength){
        mail = read();
        i++;
    }
    file.seekg(0,std::ios::beg);
    return i;
}
