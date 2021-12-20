#ifndef MODIFYWINDOW_H
#define MODIFYWINDOW_H

#include <QDialog>
#include <stdio.h>

#include "maildao.h"
#include "copiesdao.h"
#include "csvfile.h"


namespace Ui {
class ModifyWindow;
}

class ModifyWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ModifyWindow(QWidget *parent = nullptr);
    ~ModifyWindow();
    int tab;
    void clear();
    void showMail(unsigned int p);
    void showMail(unsigned int p,const char* senderSearched);
    void showCopy(Mail* mail);
    void fillFields(Mail* mail);
    unsigned int getPosCopy() const;
    void setPosCopy(unsigned int value);

private slots:

    void on_applyBT_clicked();

    void on_backBT_clicked();

    void on_idLE_textChanged(const QString &arg1);

    void on_fromLE_textChanged(const QString &arg1);

    void on_toLE_textChanged(const QString &arg1);

    void on_ccLE_textChanged(const QString &arg1);

    void on_ccoLE_textChanged(const QString &arg1);

    void on_affairLE_textChanged(const QString &arg1);

    void on_contentTE_textChanged();

    void disableFields();

    void disableApply();

private:
    Ui::ModifyWindow *ui;
    MailDao dao;
    CopiesDAO copiesDao;
    CSVFile csv;
    unsigned int idSearched,posCopy;
};

#endif // MODIFYWINDOW_H
