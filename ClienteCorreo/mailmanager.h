#ifndef MAILMANAGER_H
#define MAILMANAGER_H

#include <QTableWidget>
#include <QMessageBox>

#include "mailindex.h"
#include "maildao.h"
#include "copiesdao.h"
#include "mail.h"
#include "composewindow.h"
#include "modifywindow.h"
#include "searcheswindow.h"
#include "list.h"
#include "csvfile.h"

#define REGEXP_SEARCH "\\d{1,10}"

enum Files
{
    FILE_DATA,FILE_CSV,FILE_OWNER
};

class MailManager
{
private:

    MailDao dao;
    CopiesDAO copiesDao;
    CSVFile csv;
    MailIndex* index;
    ComposeWindow* composeWindow;
    ModifyWindow* modifyWindow;

public:
    MailManager();
    ~MailManager();
    void composeMail();
    void modifyMail(unsigned int p,QString mailSearched);
    void searchMail(QTableWidget* table,QString mailSearched);
    void searchId(QTableWidget* table,QString mailSearched);
    void searchSender(QTableWidget* table,QString mailSearched);
    void deleteEmail(unsigned int p,QString mailSearched);
    bool fileHaveData(int typeFile);
    void exportMails(int typeExport);
    void cipherMails(std::string password);
    void decipherMails();
    void importCsv(QString name);
    void importOwner();
    MailIndex*& getIndex();
};

#endif // MAILMANAGER_H
