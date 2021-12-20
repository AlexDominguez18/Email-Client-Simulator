#include "passwordbox.h"
#include "ui_passwordbox.h"

#include <QDebug>

PasswordBox::PasswordBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordBox)
{
    ui->setupUi(this);
}

PasswordBox::~PasswordBox()
{
    delete ui;
}

void PasswordBox::enableCipherBT()
{
    if (ui->passwordLE->text() == ui->repeatedPassWordLE->text()){
        ui->cipherBT->setEnabled(true);
    }
    else{
        ui->cipherBT->setEnabled(false);
    }
}

void PasswordBox::setPasswordToDecipher(std::string password)
{
    passwordToDecipher = password;
}

void PasswordBox::on_passwordLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableCipherBT();
}

void PasswordBox::on_repeatedPassWordLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableCipherBT();
}

void PasswordBox::on_cancelCipherBT_clicked()
{
    close();
}

void PasswordBox::on_cipherBT_clicked()
{
    emit cipher(ui->repeatedPassWordLE->text());
    close();
}

void PasswordBox::changePage(int i)
{
    ui->stackedWidget->setCurrentIndex(i);
}

void PasswordBox::on_decipherPassWordLE_textChanged(const QString &arg1)
{
    if (passwordToDecipher == arg1.toStdString()){
        ui->decipherBT->setEnabled(true);
    }
    else{
        ui->decipherBT->setEnabled(false);
    }
}

void PasswordBox::on_decipherBT_clicked()
{
    emit decipher(ui->decipherPassWordLE->text());
    close();
}

void PasswordBox::on_cancelBT_clicked()
{
    close();
}
