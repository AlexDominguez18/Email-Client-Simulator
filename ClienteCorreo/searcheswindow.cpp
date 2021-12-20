#include "searcheswindow.h"
#include "ui_searcheswindow.h"

#include <QDebug>

SearchesWindow::SearchesWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchesWindow)
{
    ui->setupUi(this);
    searcher = new MailSearcher();
    QPixmap logo("../Sources/logo.png");
    ui->logoLB->setPixmap(logo);
    ui->logoLB->setFixedSize(100,50);
}

SearchesWindow::~SearchesWindow()
{
    delete ui;
    delete searcher;
    index->deleteSecondaryIndex();
    if (ui->typeSearchCB->currentIndex() == 6){
        index->saveHashTable();
        index->deleteHashTable();
    }
}

void SearchesWindow::setIndex(MailIndex *&index)
{
    this->index = index;
    index->loadFileMailsSi();
}

void SearchesWindow::on_searchLE_textChanged(const QString &arg1)
{
    QRegExp regex("\\d{1,10}");
    if (regex.exactMatch(arg1) and ui->typeSearchCB->currentIndex() == 2){
        enableSearchesBT();
    }
    else if (ui->typeSearchCB->currentIndex() != 0 and ui->typeSearchCB->currentIndex() != 2){
        enableSearchesBT();
    }
    else{
        ui->searchPB->setEnabled(false);
    }
}

void SearchesWindow::enableSearchesBT()
{
    if (!ui->searchLE->text().isEmpty() and ui->typeSearchCB->currentIndex() != 0){
        ui->searchPB->setEnabled(true);
    }
    else{
        ui->searchPB->setEnabled(false);
    }
}

void SearchesWindow::on_backMenuBT_clicked()
{
    close();
    deleteLater();
}

void SearchesWindow::on_typeSearchCB_currentIndexChanged(int i)
{
    ui->searchLE->clear();
    if (ui->typeSearchCB->currentIndex() != 6){
        index->saveHashTable();
        index->deleteHashTable();
    }
    switch (i){
        case 0:
            ui->searchLE->setEnabled(false);
            break;
        case 1:
            ui->searchLE->setEnabled(true);
            ui->searchLE->setPlaceholderText("Remitente...");
            break;
        case 2:
            ui->searchLE->setEnabled(true);
            ui->searchLE->setPlaceholderText("ID...");
            break;
        case 3:
            ui->searchLE->setEnabled(true);
            ui->searchLE->setPlaceholderText("Remitente...");
            break;
        case 4:
            ui->searchLE->setEnabled(true);
            ui->searchLE->setPlaceholderText("Destinatario...");
            break;
        case 5:
            ui->searchLE->setEnabled(true);
            ui->searchLE->setPlaceholderText("Remitente AND/OR Destinatario...");
            break;
        case 6:
            ui->searchLE->setEnabled(true);
            ui->searchLE->setPlaceholderText("Remitente...");
            if (std::fstream(HASH_TABLE_FILE,std::ios::in |std::ios::binary)){
                QMessageBox::StandardButton reply = QMessageBox::question(this,"Atención",
                    "El archivo \"hashtable.dat\" existe. ¿Quiere cargarlo?",QMessageBox::Yes | QMessageBox::No);
                if (reply == QMessageBox::Yes){
                    index->loadHashTable(LOAD_HASH_FILE);
                }
                else{
                    index->loadHashTable(LOAD_MAILS_FILE);
                }
            }
            else{
                index->loadHashTable(LOAD_MAILS_FILE);
            }
            break;
    }
    enableSearchesBT();
}

void SearchesWindow::showBinarySearchResults()
{
    int row;
    Mail* mail = new Mail();
    std::string str;
    std::stringstream mails,record;

    mails = searcher->getMails(ui->searchLE->text().toStdString().c_str());
    if (!mails.str().empty()){
        while (getline(mails,str,'\r')){
            mails.ignore();
            record << str << CRLF;
            record >> *mail;
            row = ui->mailTB->rowCount();
            ui->mailTB->insertRow(row);
            ui->mailTB->setItem(row,0,new QTableWidgetItem(QString::number(mail->getId())));
            ui->mailTB->setItem(row,1,new QTableWidgetItem(QString(mail->getDate())));
            ui->mailTB->setItem(row,2,new QTableWidgetItem(QString(mail->getTime())));
            ui->mailTB->setItem(row,3,new QTableWidgetItem(QString((mail->getSender()))));
            ui->mailTB->setItem(row,4,new QTableWidgetItem(QString(mail->getAddressee())));
            ui->mailTB->setItem(row,5,new QTableWidgetItem(QString(mail->getCc())));
            ui->mailTB->setItem(row,6,new QTableWidgetItem(QString(mail->getCco())));
            ui->mailTB->setItem(row,7,new QTableWidgetItem(QString(mail->getAffair())));
            ui->mailTB->setItem(row,8,new QTableWidgetItem(QString(mail->getContent())));
        }
    }
    else{
        QMessageBox error;
        error.critical(0,"Error","No hay correos con ese remitente.");
        error.setFixedSize(600,300);
    }
}

void SearchesWindow::showPrimaryIndexResult()
{
    int row = ui->mailTB->rowCount();
    Mail* mail;
    mail = index->findMail(ui->searchLE->text().toStdString().c_str());
    if (mail != nullptr){
        ui->mailTB->insertRow(row);
        ui->mailTB->setItem(row,0,new QTableWidgetItem(QString::number(mail->getId())));
        ui->mailTB->setItem(row,1,new QTableWidgetItem(QString(mail->getDate())));
        ui->mailTB->setItem(row,2,new QTableWidgetItem(QString(mail->getTime())));
        ui->mailTB->setItem(row,3,new QTableWidgetItem(QString((mail->getSender()))));
        ui->mailTB->setItem(row,4,new QTableWidgetItem(QString(mail->getAddressee())));
        ui->mailTB->setItem(row,5,new QTableWidgetItem(QString(mail->getCc())));
        ui->mailTB->setItem(row,6,new QTableWidgetItem(QString(mail->getCco())));
        ui->mailTB->setItem(row,7,new QTableWidgetItem(QString(mail->getAffair())));
        ui->mailTB->setItem(row,8,new QTableWidgetItem(QString(mail->getContent())));
    }
    else{
        QMessageBox error;
        error.critical(0,"Error","No hay correos con ese ID.");
        error.setFixedSize(600,300);
    }
}

void SearchesWindow::showSecondaryIndexSenderResults()
{
    int row;
    Mail mail;
    List<Mail>& mails = index->getMailsSender(ui->searchLE->text().toStdString().c_str());
    qDebug() << "Lista retornada";
    List<Mail>::Node* aux(mails.getFirstPos());
    qDebug() << "Nodo retornado";
    if (aux != nullptr){
        while (aux != nullptr){
            row = ui->mailTB->rowCount();
            ui->mailTB->insertRow(row);
            mail = aux->getData();
            ui->mailTB->setItem(row,0,new QTableWidgetItem(QString::number(mail.getId())));
            ui->mailTB->setItem(row,1,new QTableWidgetItem(QString(mail.getDate())));
            ui->mailTB->setItem(row,2,new QTableWidgetItem(QString(mail.getTime())));
            ui->mailTB->setItem(row,3,new QTableWidgetItem(QString((mail.getSender()))));
            ui->mailTB->setItem(row,4,new QTableWidgetItem(QString(mail.getAddressee())));
            ui->mailTB->setItem(row,5,new QTableWidgetItem(QString(mail.getCc())));
            ui->mailTB->setItem(row,6,new QTableWidgetItem(QString(mail.getCco())));
            ui->mailTB->setItem(row,7,new QTableWidgetItem(QString(mail.getAffair())));
            ui->mailTB->setItem(row,8,new QTableWidgetItem(QString(mail.getContent())));
            aux = aux->getNext();
        }
    }
    else{
        QMessageBox error;
        error.critical(0,"Error","No hay correos con ese remitente.");
        error.setFixedSize(600,300);
    }
}

void SearchesWindow::showSecondaryIndexAddreseeResults()
{
    int row;
    Mail mail;
    List<Mail>& mails = index->getMailsAddressee(ui->searchLE->text().toStdString().c_str());
    qDebug() << "Lista retornada";
    List<Mail>::Node* aux(mails.getFirstPos());
    qDebug() << "Nodo retornado";
    if (aux != nullptr){
        while (aux != nullptr){
            row = ui->mailTB->rowCount();
            ui->mailTB->insertRow(row);
            mail = aux->getData();
            ui->mailTB->setItem(row,0,new QTableWidgetItem(QString::number(mail.getId())));
            ui->mailTB->setItem(row,1,new QTableWidgetItem(QString(mail.getDate())));
            ui->mailTB->setItem(row,2,new QTableWidgetItem(QString(mail.getTime())));
            ui->mailTB->setItem(row,3,new QTableWidgetItem(QString((mail.getSender()))));
            ui->mailTB->setItem(row,4,new QTableWidgetItem(QString(mail.getAddressee())));
            ui->mailTB->setItem(row,5,new QTableWidgetItem(QString(mail.getCc())));
            ui->mailTB->setItem(row,6,new QTableWidgetItem(QString(mail.getCco())));
            ui->mailTB->setItem(row,7,new QTableWidgetItem(QString(mail.getAffair())));
            ui->mailTB->setItem(row,8,new QTableWidgetItem(QString(mail.getContent())));
            aux = aux->getNext();
        }
    }
    else{
        QMessageBox error;
        error.critical(0,"Error","No hay correos con ese destinatario.");
        error.setFixedSize(600,300);
    }
}

void SearchesWindow::showSecondaryIndexBooleanResults()
{
    int row;
    Mail mail;
    List<Mail>& mails = index->getMailsBoolean(ui->searchLE->text().toStdString());
    qDebug() << "Lista retornada";
    List<Mail>::Node* aux(mails.getFirstPos());
    qDebug() << "Nodo retornado";
    if (aux != nullptr){
        while (aux != nullptr){
            row = ui->mailTB->rowCount();
            ui->mailTB->insertRow(row);
            mail = aux->getData();
            ui->mailTB->setItem(row,0,new QTableWidgetItem(QString::number(mail.getId())));
            ui->mailTB->setItem(row,1,new QTableWidgetItem(QString(mail.getDate())));
            ui->mailTB->setItem(row,2,new QTableWidgetItem(QString(mail.getTime())));
            ui->mailTB->setItem(row,3,new QTableWidgetItem(QString((mail.getSender()))));
            ui->mailTB->setItem(row,4,new QTableWidgetItem(QString(mail.getAddressee())));
            ui->mailTB->setItem(row,5,new QTableWidgetItem(QString(mail.getCc())));
            ui->mailTB->setItem(row,6,new QTableWidgetItem(QString(mail.getCco())));
            ui->mailTB->setItem(row,7,new QTableWidgetItem(QString(mail.getAffair())));
            ui->mailTB->setItem(row,8,new QTableWidgetItem(QString(mail.getContent())));
            aux = aux->getNext();
        }
    }
    else{
        QMessageBox error;
        error.critical(0,"Error","No hay correos con ese criterio.");
        error.setFixedSize(600,300);
    }
}

void SearchesWindow::showHashMailsResults()
{
    int row;
    Mail mail;
    List<Mail>& mails = index->findHashMail((char*)ui->searchLE->text().toStdString().c_str());
    qDebug() << "Lista retornada";
    List<Mail>::Node* aux(mails.getFirstPos());
    qDebug() << "Nodo retornado";
    if (aux != nullptr){
        while (aux != nullptr){
            row = ui->mailTB->rowCount();
            ui->mailTB->insertRow(row);
            mail = aux->getData();
            ui->mailTB->setItem(row,0,new QTableWidgetItem(QString::number(mail.getId())));
            ui->mailTB->setItem(row,1,new QTableWidgetItem(QString(mail.getDate())));
            ui->mailTB->setItem(row,2,new QTableWidgetItem(QString(mail.getTime())));
            ui->mailTB->setItem(row,3,new QTableWidgetItem(QString((mail.getSender()))));
            ui->mailTB->setItem(row,4,new QTableWidgetItem(QString(mail.getAddressee())));
            ui->mailTB->setItem(row,5,new QTableWidgetItem(QString(mail.getCc())));
            ui->mailTB->setItem(row,6,new QTableWidgetItem(QString(mail.getCco())));
            ui->mailTB->setItem(row,7,new QTableWidgetItem(QString(mail.getAffair())));
            ui->mailTB->setItem(row,8,new QTableWidgetItem(QString(mail.getContent())));
            aux = aux->getNext();
        }
    }
    else{
        QMessageBox error;
        error.critical(0,"Error","No hay correos con ese criterio.");
        error.setFixedSize(600,300);
    }
}

void SearchesWindow::on_searchPB_clicked()
{
    ui->mailTB->clearContents();
    ui->mailTB->setRowCount(0);
    switch (ui->typeSearchCB->currentIndex()){
        case 1:
            showBinarySearchResults();
            break;
        case 2:
            showPrimaryIndexResult();
            break;
        case 3:
            showSecondaryIndexSenderResults();
            break;
        case 4:
            showSecondaryIndexAddreseeResults();
            break;
        case 5:
            showSecondaryIndexBooleanResults();
            break;
        case 6:
            showHashMailsResults();
            break;
    }
    ui->searchLE->clear();
}
