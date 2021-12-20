#ifndef COPIESDAO_H
#define COPIESDAO_H

#include <fstream>

#include "mail.h"

#define COPIES_FILE "../Files/mailsCopies.dat"
#define TMP_COPIES "../Files/tmp.dat"
#define NULL_CHAR '\0'

class CopiesDAO
{
private:
    std::fstream file;
public:
    CopiesDAO();
    ~CopiesDAO();
    bool haveData();
    void openFile();
    void closeFile();
    void write(Mail* mail);
    void write(Mail* mail,std::fstream& tmpFile);
    Mail* read();
    Mail *getById(unsigned int id);
    void update(Mail* mail);
    void deleteCopy(unsigned int id);
    unsigned int getFileLength();
    unsigned int getRecordsCounter();
};

#endif // COPIESDAO_H
