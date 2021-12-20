#ifndef PASSWORDBOX_H
#define PASSWORDBOX_H

#include <QDialog>

namespace Ui {
class PasswordBox;
}

class PasswordBox : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordBox(QWidget *parent = nullptr);
    ~PasswordBox();
    void enableCipherBT();
    void setPasswordToDecipher(std::string password);

public slots:
    void changePage(int i);

private slots:
    void on_passwordLE_textChanged(const QString &arg1);

    void on_repeatedPassWordLE_textChanged(const QString &arg1);

    void on_cancelCipherBT_clicked();

    void on_cipherBT_clicked();

    void on_decipherBT_clicked();

    void on_decipherPassWordLE_textChanged(const QString &arg1);

    void on_cancelBT_clicked();

signals:
    void cipher(const QString& password);

    void decipher(const QString& password);

private:
    Ui::PasswordBox *ui;
    std::string passwordToDecipher;
};

#endif // PASSWORDBOX_H
