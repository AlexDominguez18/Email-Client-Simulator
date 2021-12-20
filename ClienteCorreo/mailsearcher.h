#ifndef MAILSEARCHER_H
#define MAILSEARCHER_H

#include <sstream>

#include "mail.h"
#include "maildao.h"

#define ARRAY_SIZE 10000

class MailSearcher
{
private:
    unsigned int mailsCount;
    Mail* mails[ARRAY_SIZE];
    MailDao mailDao;
public:
    MailSearcher();
    ~MailSearcher();
    void loadMails();
    void shellSort();
    long long binarySearch(const char* senderSearched);
    std::stringstream getMails(const char* senderSearched);
};

#endif // MAILSEARCHER_H
