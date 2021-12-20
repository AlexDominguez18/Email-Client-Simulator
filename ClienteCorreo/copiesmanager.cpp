#include "copiesmanager.h"

#include <QDebug>

CopiesManager::CopiesManager(){ }

CopiesManager::~CopiesManager(){    }

bool CopiesManager::showCopiesTable(QTableWidget *&table,unsigned int id)
{
    Mail* mail;
    mail = getCopy(id);
    if (mail != nullptr){
        table->setItem(ID,0,new QTableWidgetItem(QString::number(mail->getId())));
        table->setItem(DATE,0,new QTableWidgetItem(QString(mail->getDate())));
        table->setItem(TIME,0,new QTableWidgetItem(QString(mail->getTime())));
        table->setItem(SENDER,0,new QTableWidgetItem(QString(mail->getSender())));
        table->setItem(ADDRESSEES,0,new QTableWidgetItem(QString(mail->getAddressee())));
        table->setItem(AFFAIR,0,new QTableWidgetItem(QString(mail->getAffair())));
        table->setItem(CONTENT,0,new QTableWidgetItem(QString(mail->getContent())));
        return true;
    }
    return false;
}

bool CopiesManager::showCopiesPTable(QTableWidget *&table, unsigned int id)
{
    Mail* mail;
    dao.openFile();
    mail = dao.getById(id);
    dao.closeFile();
    if (mail != nullptr){
        table->setItem(ID,0,new QTableWidgetItem(QString::number(mail->getId())));
        table->setItem(DATE,0,new QTableWidgetItem(QString(mail->getDate())));
        table->setItem(TIME,0,new QTableWidgetItem(QString(mail->getTime())));
        table->setItem(SENDER,0,new QTableWidgetItem(QString(mail->getSender())));
        table->setItem(ADDRESSEES,0,new QTableWidgetItem(QString(mail->getAddressee())));
        table->setItem(AFFAIR,0,new QTableWidgetItem(QString(mail->getAffair())));
        table->setItem(CONTENT,0,new QTableWidgetItem(QString(mail->getContent())));
        return true;
    }
    return false;
}

void CopiesManager::modifyCopy(unsigned int id,int typeFile)
{
    Mail* mail = new Mail();
    unsigned int pos;
    modifyWindow = new ModifyWindow();

    modifyWindow->setModal(true);

    switch (typeFile){
        case 0:
            modifyWindow->tab = 0;
            modifyWindow->showCopy(mail);
            break;
        case 1:
            modifyWindow->tab = 1;
            mail = getCopy(id);
            pos = getPosCopy(mail);
            modifyWindow->setPosCopy(pos);
            modifyWindow->showCopy(mail);
            break;
        case 2:
            modifyWindow->tab = 2;
            dao.openFile();
            mail = dao.getById(id);
            dao.closeFile();
            modifyWindow->showCopy(mail);
            break;
    }
    modifyWindow->exec();
    delete modifyWindow;
}

Mail* CopiesManager::getCopy(unsigned int id)
{
    Mail* mail = new Mail();
    std::string str;
    std::stringstream ss,record;
    unsigned int copiesCounter;

     /*csv.file.ignore();
    getline(csv.file,str,'\r');
    csv.file.ignore();
    i = 0;
    while (i < copiesCounter){
        getline(csv.file,str,DELIMITER);
        p = atol(str.c_str());
        if (p == id){
            getline(csv.file,str,'\r');
            csv.file.ignore();
            record = std::to_string(p) + DELIMITER + csv.parseCsvRecord(str);
            ss << record;
            ss >> *mail;
            csv.file.close();
            return mail;
        }
        getline(csv.file,str,'\r');
        csv.file.ignore();
        i++;
    }
    csv.file.close();
    return nullptr;*/
    csv.file.open(CSV_FILE,std::ios::in);
    getline(csv.file,str,'\r');
    copiesCounter = atol(str.c_str());
    csv.file.close();
    ss = csv.parseRecords();
    for (unsigned int i = 0; i < copiesCounter;i++){
        getline(ss,str,'\r');
        ss.ignore();
        record << str << CRLF;
        record >> *mail;
        if (mail->getId() == id){
             return mail;
        }
    }
    return nullptr;
}

void CopiesManager::deleteCopy(unsigned int id,int typeFile)
{
    Mail* mail = new Mail();
    unsigned int pos;
    switch (typeFile){
        case 1:
            mail = getCopy(id);
            pos = getPosCopy(mail);
            csv.deleteRecord(pos);
            break;
        case 2:
            dao.openFile();
            dao.deleteCopy(id);
            dao.closeFile();
            break;
    }
}

int CopiesManager::getPosCopy(Mail *mail)
{
    unsigned int p,i,id,copiesCounter;
    std::string str;
    csv.file.open(CSV_FILE,std::ios::in);
    //Remove counter and header
    getline(csv.file,str,'\r');
    csv.file.ignore();
    copiesCounter = atol(str.c_str());
    getline(csv.file,str,'\r');
    csv.file.ignore();
    i = 0;
    id = mail->getId();
    while (i < copiesCounter){
        getline(csv.file,str,DELIMITER);
        csv.file.ignore();
        p = atol(str.c_str());
        if (p == id){
            csv.file.close();
            return i;
        }
        getline(csv.file,str,'\r');
        csv.file.ignore();
        i++;
    }
    csv.file.close();
    return -1;
}
