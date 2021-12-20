#include "mailmanager.h"
#include <QDebug>

MailManager::MailManager()
{
    index = new MailIndex();
}

MailManager::~MailManager()
{
    delete index;
}

void MailManager::composeMail()
{
    composeWindow = new ComposeWindow();
    composeWindow->setModal(true);
    composeWindow->setIndex(index);
    composeWindow->exec();
    delete composeWindow;
}

void MailManager::modifyMail(unsigned int p,QString mailSearched)
{
    modifyWindow = new ModifyWindow();
    QRegExp typeModify(REGEXP_SEARCH);

    modifyWindow->tab = 0;
    if (typeModify.exactMatch(mailSearched)){
        modifyWindow->setModal(true);
        modifyWindow->showMail(p);
        modifyWindow->exec();
    }
    else{
        modifyWindow->setModal(true);
        modifyWindow->showMail(p,mailSearched.toStdString().c_str());
        modifyWindow->exec();
    }
    delete modifyWindow;
}

void MailManager::searchMail(QTableWidget* table,QString mailSearched)
{
    QRegExp typeSearch("\\d{1,10}");

    if (typeSearch.exactMatch(mailSearched)){
        searchId(table,mailSearched);
    }
    else{
        searchSender(table,mailSearched);
    }
}

void MailManager::searchId(QTableWidget* table,QString mailSearched)
{
    Mail* mail= new Mail();
    unsigned int idSearched;
    idSearched = mailSearched.toULong();
    mail = dao.getById(idSearched);
    table->setRowCount(0);

    if (mail != nullptr){
        int row = table->rowCount();
        table->insertRow(row);
        table->setItem(row,ID,new QTableWidgetItem(QString::number(mail->getId())));
        table->setItem(row,DATE,new QTableWidgetItem(QString(mail->getDate())));
        table->setItem(row,TIME,new QTableWidgetItem(QString(mail->getTime())));
        table->setItem(row,SENDER,new QTableWidgetItem(QString((mail->getSender()))));
        table->setItem(row,ADDRESSEES,new QTableWidgetItem(QString(mail->getAddressee())));
        table->setItem(row,AFFAIR,new QTableWidgetItem(QString(mail->getAffair())));
    }
    else{
        QMessageBox error;
        error.critical(0,"Error","No existe correo con ese ID");
        error.setFixedSize(600,300);
    }
    delete mail;
}

void MailManager::searchSender(QTableWidget* table,QString mailSearched)
{
    int row = 0;
    Mail mail;
    List<Mail> list;
    dao.getBySender(mailSearched.toStdString().c_str(),list);
    List<Mail>::Node* aux(list.getFirstPos());
    table->setRowCount(0);
    if (aux != nullptr){
        while (aux != nullptr){
            mail = aux->getData();
            table->insertRow(table->rowCount());
            table->setItem(row,ID,new QTableWidgetItem(QString::number(mail.getId())));
            table->setItem(row,DATE,new QTableWidgetItem(QString(mail.getDate())));
            table->setItem(row,TIME,new QTableWidgetItem(QString(mail.getTime())));
            table->setItem(row,SENDER,new QTableWidgetItem(QString((mail.getSender()))));
            table->setItem(row,ADDRESSEES,new QTableWidgetItem(QString(mail.getAddressee())));
            table->setItem(row,AFFAIR,new QTableWidgetItem(QString(mail.getAffair())));
            aux = aux->getNext();
            row++;
        }
    }
    else{
        QMessageBox error;
        error.critical(0,"Error","No hay correos con ese remitente.");
        error.setFixedSize(600,300);
    }
    list.deleteAll();
}

void MailManager::deleteEmail(unsigned int p,QString mailSearched)
{
    QRegExp typeDelete(REGEXP_SEARCH);

    if (typeDelete.exactMatch(mailSearched)){
        dao.deleteById(p);
        //Deleting Primary Index of the Mail Index
        PrimaryIndex pi(std::to_string(p).c_str());
        index->deletePrimaryIndex(pi);
        index->primaryIndexUpdate(false);
    }
    else{
        dao.deleteBySender(p,mailSearched.toStdString().c_str());
    }
}

bool MailManager::fileHaveData(int typeFile)
{
    switch (typeFile) {
        case FILE_DATA:
            return dao.haveData();
        case FILE_CSV:
            return csv.haveData();
        case FILE_OWNER:
            return copiesDao.haveData();
        default:
            return false;
    }
    return false;
}

void MailManager::exportMails(int typeExport)
{
    unsigned int i,fileSize,mailsCounter = 0;
    Mail* mail;
    std::string str;
    std::stringstream ss;
    fileSize = dao.getFileLength();
    switch (typeExport){
    case FILE_CSV:
        i = 1;
        while(i <= fileSize){
            mail = dao.getById(i);
            if (mail != nullptr){
                str = mail->getSender();
                mail->setSender(csv.toCsv(str).c_str());
                str = mail->getAddressee();
                mail->setAddressee(csv.toCsv(str).c_str());
                str = mail->getCc();
                mail->setCc(csv.toCsv(str).c_str());
                str = mail->getCco();
                mail->setCco(csv.toCsv(str).c_str());
                str = mail->getAffair();
                mail->setAffair(csv.toCsv(str).c_str());
                str = mail->getContent();
                mail->setContent(csv.toCsv(str).c_str());
                ss << mail->toString();
                mailsCounter++;
            }
            i++;
        }
        csv.write(mailsCounter,ss.str());
        break;
    case FILE_OWNER:
        i = 1;
        copiesDao.openFile();
        while (i <= fileSize){
            mail = dao.getById(i);
            if (mail != nullptr){
                copiesDao.write(mail);
            }
            i++;
        }
        copiesDao.closeFile();
        break;
    }
}

void MailManager::cipherMails(std::string password)
{
    std::fstream tmp;
    tmp.open(TMP_FILE,std::ios::out);
    if (tmp.is_open()){
        csv.file.open(CSV_FILE,std::ios::in | std::ios::out);
        tmp << -1 << CRLF;
        tmp << csv.encrypt(password) << CRLF;
        while (!csv.file.eof()){
            char c;
            csv.file.get(c);
            tmp << csv.cipherCaesar(c);
        }
        tmp.close();
        csv.file.close();
        remove(CSV_FILE);
        rename(TMP_FILE,CSV_FILE);
    }
}

void MailManager::decipherMails()
{
    std::fstream decipher;
    decipher.open(TMP_FILE,std::ios::out);
    if (decipher.is_open()){
        std::fstream tmp;
        tmp.open(CSV_FILE,std::ios::in | std::ios::out);
        int isEncrypted;
        std::string password;
        tmp >> isEncrypted;
        tmp >> password;
        tmp.get();
        tmp.get();
        while (!tmp.eof()){
            char c;
            tmp.get(c);
            decipher << csv.decipherCaesar(c);
        }
        tmp.close();
        decipher.close();
        remove(CSV_FILE);
        rename(TMP_FILE,CSV_FILE);
    }
}

void MailManager::importCsv(QString name)
{
    Mail* mail = new Mail();
    unsigned int counter,i;
    std::string str;
    std::stringstream ss,record;

    csv.file.open(name.toStdString(),std::ios::in);
    getline(csv.file,str,'\r');
    counter = atol(str.c_str());
    csv.file.close();
    ss = csv.parseRecords();
    i = 0;
    while (i < counter){
        getline(ss,str,'\r');
        ss.ignore();
        record << str << CRLF;
        qDebug() << "Record = " << record.str().c_str();
        record >> *mail;
        if (dao.getById(mail->getId()) != nullptr){
            QMessageBox msg;
            std::string text = "El registro: " + std::to_string(mail->getId()) + " Ya existe"
                                "\n¿Quiere sobreescribirlo?";
            msg.setWindowTitle("Atención");
            msg.setText(text.c_str());
            msg.setStandardButtons(QMessageBox::Yes);
            msg.addButton(QMessageBox::No);
            msg.setDefaultButton(QMessageBox::No);
            if (msg.exec() == QMessageBox::Yes){
                dao.update(mail,mail->getId());
            }
        }
        else{
            dao.add(mail);
        }
        i++;
    }
    csv.file.close();
}

void MailManager::importOwner()
{
    Mail* mail = new Mail();
    unsigned int i,recordsCounter;
    copiesDao.openFile();
    recordsCounter = copiesDao.getRecordsCounter();
    i = 0;
    while (i < recordsCounter){
        mail = copiesDao.read();
        if (dao.getById(mail->getId()) != nullptr){
            QMessageBox msg;
            std::string text = "El registro: " + std::to_string(mail->getId()) + " Ya existe"
                                "\n¿Quiere sobreescribirlo?";
            msg.setWindowTitle("Atención");
            msg.setText(text.c_str());
            msg.setStandardButtons(QMessageBox::Yes);
            msg.addButton(QMessageBox::No);
            msg.setDefaultButton(QMessageBox::No);
            if (msg.exec() == QMessageBox::Yes){
                dao.update(mail,mail->getId());
            }
        }
        else{
            dao.add(mail);
        }
        i++;
    }
    copiesDao.closeFile();
}

MailIndex *&MailManager::getIndex()
{
    return index;
}
