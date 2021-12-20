#include "modifywindow.h"
#include "ui_modifywindow.h"

#include <QDebug>

ModifyWindow::ModifyWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModifyWindow)
{
    ui->setupUi(this);
}

ModifyWindow::~ModifyWindow()
{
    delete ui;
}

void ModifyWindow::clear()
{
    ui->idLE->clear();
    ui->dateLE->clear();
    ui->timeLE->clear();
    ui->toLE->clear();
    ui->fromLE->clear();
    ui->ccLE->clear();
    ui->ccoLE->clear();
    ui->affairLE->clear();
    ui->contentTE->clear();
}

void ModifyWindow::on_applyBT_clicked()
{
    Mail* mail = new Mail();
    std::string str;

    switch (tab){
        case 0:
            mail->setId(ui->idLE->text().toULong());
            mail->setDate(ui->dateLE->text().toStdString().c_str());
            mail->setTime(ui->timeLE->text().toStdString().c_str());
            mail->setSender(ui->fromLE->text().toStdString().c_str());
            mail->setAddressee(ui->toLE->text().toStdString().c_str());
            mail->setCc(ui->ccLE->text().toStdString().c_str());
            mail->setCco(ui->ccoLE->text().toStdString().c_str());
            mail->setAffair(ui->affairLE->text().toStdString().c_str());
            mail->setContent(ui->contentTE->toPlainText().toStdString().c_str());
            dao.update(mail,mail->getId());
            break;
        case 1:
            mail->setId(ui->idLE->text().toULong());
            str = ui->dateLE->text().toStdString();
            mail->setDate(csv.toCsv(str).c_str());
            str = ui->timeLE->text().toStdString();
            mail->setTime(csv.toCsv(str).c_str());
            str = ui->fromLE->text().toStdString();
            mail->setSender(csv.toCsv(str).c_str());
            str = ui->toLE->text().toStdString();
            mail->setAddressee(csv.toCsv(str).c_str());
            str = ui->ccLE->text().toStdString();
            mail->setCc(csv.toCsv(str).c_str());
            str = ui->ccoLE->text().toStdString();
            mail->setCco(csv.toCsv(str).c_str());
            str = ui->affairLE->text().toStdString();
            mail->setAffair(csv.toCsv(str).c_str());
            str = ui->contentTE->toPlainText().toStdString();
            mail->setContent(csv.toCsv(str).c_str());
            str = mail->toString();
            csv.update(posCopy,str);
        break;
    case 2:
        mail->setId(ui->idLE->text().toULong());
        mail->setDate(ui->dateLE->text().toStdString().c_str());
        mail->setTime(ui->timeLE->text().toStdString().c_str());
        mail->setSender(ui->fromLE->text().toStdString().c_str());
        mail->setAddressee(ui->toLE->text().toStdString().c_str());
        mail->setCc(ui->ccLE->text().toStdString().c_str());
        mail->setCco(ui->ccoLE->text().toStdString().c_str());
        mail->setAffair(ui->affairLE->text().toStdString().c_str());
        mail->setContent(ui->contentTE->toPlainText().toStdString().c_str());
        copiesDao.openFile();
        copiesDao.update(mail);
        copiesDao.closeFile();
        break;
    }
    clear();
    close();
}

void ModifyWindow::on_backBT_clicked()
{
    reject();
}

//DISABLE OR ENABLE THE FIELDS

void ModifyWindow::on_idLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    disableApply();
}

void ModifyWindow::on_fromLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    disableApply();
}

void ModifyWindow::on_toLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    disableApply();
}

void ModifyWindow::on_ccLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    disableApply();
}

void ModifyWindow::on_ccoLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    disableApply();
}

void ModifyWindow::on_affairLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    disableApply();
}

void ModifyWindow::on_contentTE_textChanged()
{
    disableApply();
}

void ModifyWindow::disableApply()
{
    if (!ui->idLE->text().isEmpty() and !ui->fromLE->text().isEmpty()
            and !ui->toLE->text().isEmpty() and !ui->ccLE->text().isEmpty()
            and !ui->ccoLE->text().isEmpty() and !ui->affairLE->text().isEmpty()
            and !ui->contentTE->toPlainText().isEmpty()){
        ui->applyBT->setEnabled(true);

    }
    else{
        ui->applyBT->setEnabled(false);
    }
}

unsigned int ModifyWindow::getPosCopy() const
{
    return posCopy;
}

void ModifyWindow::setPosCopy(unsigned int value)
{
    posCopy = value;
}

void ModifyWindow::disableFields()
{
    ui->idLE->setEnabled(true);
    ui->fromLE->setEnabled(true);
    ui->toLE->setEnabled(true);
    ui->ccLE->setEnabled(true);
    ui->ccoLE->setEnabled(true);
    ui->affairLE->setEnabled(true);
    ui->contentTE->setEnabled(true);
}

void ModifyWindow::showMail(unsigned int p)
{
    Mail* mail = new Mail();
    QString s;
    idSearched = p;
    mail = dao.getById(idSearched);
    if (mail != nullptr){
        fillFields(mail);
    }
    else{
        QMessageBox error;
        error.critical(0,"Error","No existe un correo con ese ID.");
        error.setFixedSize(600,300);
    }
    delete mail;
}

void ModifyWindow::showMail(unsigned int p ,const char *senderSearched)
{
    Mail* mail = new Mail();
    QString s;
    mail = dao.getBySender(p,senderSearched);
    if (mail != nullptr){
        fillFields(mail);
    }
    else{
        QMessageBox error;
        error.critical(0,"Error","No existe un correo con ese remitente.");
        error.setFixedSize(600,300);
    }
    delete mail;
}

void ModifyWindow::showCopy(Mail* mail)
{
    fillFields(mail);
}

void ModifyWindow::fillFields(Mail *mail)
{
    QString lineEdit;
    disableFields();
    lineEdit = QString::number(mail->getId());
    ui->idLE->setText(lineEdit);
    lineEdit = mail->getSender();
    ui->fromLE->setText(lineEdit);
    lineEdit = mail->getAddressee();
    ui->toLE->setText(lineEdit);
    lineEdit = mail->getCc();
    ui->ccLE->setText(lineEdit);
    lineEdit = mail->getCco();
    ui->ccoLE->setText(lineEdit);
    lineEdit = mail->getAffair();
    ui->affairLE->setText(lineEdit);
    lineEdit = mail->getContent();
    ui->contentTE->setPlainText(lineEdit);
    lineEdit = mail->getDate();
    ui->dateLE->setText(lineEdit);
    lineEdit = mail->getTime();
    ui->timeLE->setText(lineEdit);
}
