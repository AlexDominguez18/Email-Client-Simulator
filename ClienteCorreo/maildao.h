#ifndef MAILDAO_H
#define MAILDAO_H

#include <QMessageBox>
#include <fstream>

#include "mail.h"
#include "list.h"

#define MAIL_FILE "../Files/mails.dat"

class MailDao
{
private:
    std::fstream file;
public:
    MailDao();
    ~MailDao();
    bool add(Mail* m);
    void update(Mail* m,unsigned int p);
    Mail* getByPos(unsigned int p);
    Mail* getById(unsigned int p);
    Mail* getBySender(unsigned int p,const char* senderSearched);
    void getBySender(const char* senderSearched,List<Mail>& mails);
    void deleteById(unsigned int p);
    void deleteBySender(unsigned int p,const char* senderSearched);
    unsigned int getFileLength();
    bool haveData();
};

#endif // MAILDAO_H
