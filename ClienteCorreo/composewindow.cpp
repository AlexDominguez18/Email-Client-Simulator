#include "composewindow.h"
#include "ui_composewindow.h"

#include <iostream>
#include <string>
#include <QDebug>

ComposeWindow::ComposeWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComposeWindow)
{
    ui->setupUi(this);
}

ComposeWindow::~ComposeWindow()
{
    delete ui;
}

void ComposeWindow::setIndex(MailIndex *&mailIndex)
{
    this->mailIndex = mailIndex;
}

void ComposeWindow::clear()
{
    ui->idLE->clear();
    ui->toLE->clear();
    ui->fromLE->clear();
    ui->ccLE->clear();
    ui->ccoLE->clear();
    ui->affairLE->clear();
    ui->contentTE->clear();
}

void ComposeWindow::on_exitB_accepted()
{
    Mail* mail = new Mail();
    QMessageBox message;
    //Object Instance
    if (ui->idLE->text().toULong() > 0){
        mail->setId(ui->idLE->text().toULong());
        mail->setSender(ui->fromLE->text().toStdString().c_str());
        mail->setAddressee(ui->toLE->text().toStdString().c_str());
        mail->setCc(ui->ccLE->text().toStdString().c_str());
        mail->setCco(ui->ccoLE->text().toStdString().c_str());
        mail->setAffair(ui->affairLE->text().toStdString().c_str());
        mail->setContent(ui->contentTE->toPlainText().toStdString().c_str());
        //Saving Object
        if (dao.add(mail)){
            //Updating Primary's Index file state
            PrimaryIndex pi;
            unsigned int pos = mail->getId()-1;
            pi.setKey(std::to_string(mail->getId()).c_str());
            pi.setValue(pos*sizeof(Mail));
            qDebug() << "Insertando Indice";
            mailIndex->insertPrimaryIndex(pi);
            mailIndex->primaryIndexUpdate(false);
            message.setText("¡Se ha añadido el correo!");
            message.setIcon(QMessageBox::Information);
            message.setFixedSize(500,200);
            message.exec();
        }
        else{
            message.critical(0,"Error","Ya existe ese ID.");
            message.setFixedSize(500,200);
        }
    }
    else{
        message.setText("No se puede agregar el correo con ese ID.");
        message.setIcon(QMessageBox::Critical);
        message.exec();
    }
    clear();
    close();
}

void ComposeWindow::on_exitB_rejected()
{
    close();
}

void ComposeWindow::disableButton()
{
    if (!ui->idLE->text().isEmpty() and !ui->fromLE->text().isEmpty()
        and !ui->toLE->text().isEmpty() and !ui->ccLE->text().isEmpty()
        and !ui->ccoLE->text().isEmpty() and !ui->affairLE->text().isEmpty()
        and !ui->contentTE->toPlainText().isEmpty()){
                ui->exitB->setEnabled(true);
    }
    else{
        ui->exitB->setEnabled(false);
    }
}

void ComposeWindow::on_idLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    disableButton();
}

void ComposeWindow::on_fromLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    disableButton();
}

void ComposeWindow::on_toLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    disableButton();
}

void ComposeWindow::on_ccLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    disableButton();
}

void ComposeWindow::on_ccoLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    disableButton();
}

void ComposeWindow::on_affairLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    disableButton();
}

void ComposeWindow::on_contentTE_textChanged()
{
    disableButton();
}
