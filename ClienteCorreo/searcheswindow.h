#ifndef SEARCHESWINDOW_H
#define SEARCHESWINDOW_H

#include <QWidget>
#include <iostream>
#include <sstream>

#include "mailsearcher.h"
#include "mailindex.h"

namespace Ui {
class SearchesWindow;
}

class SearchesWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SearchesWindow(QWidget *parent = nullptr);
    ~SearchesWindow();
    void setIndex(MailIndex*& index);

private slots:

    void on_searchLE_textChanged(const QString &arg1);

    void enableSearchesBT();

    void on_backMenuBT_clicked();

    void on_searchPB_clicked();

    void on_typeSearchCB_currentIndexChanged(int index);

    void showBinarySearchResults();

    void showPrimaryIndexResult();

    void showSecondaryIndexSenderResults();

    void showSecondaryIndexAddreseeResults();

    void showSecondaryIndexBooleanResults();

    void showHashMailsResults();

private:

    Ui::SearchesWindow *ui;
    MailSearcher* searcher;
    MailIndex* index;
};

#endif // SEARCHESWINDOW_H
