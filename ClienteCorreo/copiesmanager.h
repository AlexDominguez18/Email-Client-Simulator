#ifndef COPIESMANAGER_H
#define COPIESMANAGER_H

#include <QTableWidget>

#include "csvfile.h"
#include "copiesdao.h"
#include "modifywindow.h"

class CopiesManager
{
private:
    CSVFile csv;
    CopiesDAO dao;
    ModifyWindow* modifyWindow;

public:
    CopiesManager();
    ~CopiesManager();
    bool showCopiesTable(QTableWidget*& table,unsigned int id);
    bool showCopiesPTable(QTableWidget*& table,unsigned int id);
    void modifyCopy(unsigned int id,int typeFile);
    Mail* getCopy(unsigned int id);
    void deleteCopy(unsigned int id,int typeFile);
    int getPosCopy(Mail* mail);
};

#endif // COPIESMANAGER_H
