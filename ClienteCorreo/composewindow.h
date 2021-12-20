#ifndef COMPOSEWINDOW_H
#define COMPOSEWINDOW_H

#include <QDialog>
#include <QAbstractButton>

#include "maildao.h"
#include "mailindex.h"

namespace Ui {
class ComposeWindow;
}

class ComposeWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ComposeWindow(QWidget *parent = nullptr);
    ~ComposeWindow();
    void setIndex(MailIndex*& mailIndex);

private slots:
    void on_exitB_accepted();

    void disableButton();

    void clear();

    void on_idLE_textChanged(const QString &arg1);

    void on_fromLE_textChanged(const QString &arg1);

    void on_toLE_textChanged(const QString &arg1);

    void on_ccLE_textChanged(const QString &arg1);

    void on_ccoLE_textChanged(const QString &arg1);

    void on_affairLE_textChanged(const QString &arg1);

    void on_contentTE_textChanged();

    void on_exitB_rejected();

private:
    Ui::ComposeWindow *ui;
    MailDao dao;
    MailIndex* mailIndex;
};

#endif // COMPOSEWINDOW_H
