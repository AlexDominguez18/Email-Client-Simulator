#include "mailsearcher.h"

#include <QDebug>

MailSearcher::MailSearcher()
{
    loadMails();
    shellSort();
}

MailSearcher::~MailSearcher()
{
    //Deleting the mails
    for (unsigned int i = 0; i < mailsCount; ++i){
        delete mails[i];
    }
}

void MailSearcher::loadMails()
{
    unsigned int i,j, fileLength;
    fileLength = mailDao.getFileLength();
    for (mailsCount = i = j = 0; i < fileLength;i++){
        mails[j] = new Mail();
        mails[j] = mailDao.getById(i+1);
        if (mails[j] != nullptr){
            j++;
            mailsCount++;
        }
    }
}

void MailSearcher::shellSort()
{
    long long gap, i, j , k;

    gap = mailsCount/2;

    while (gap > 0){
        for (i = gap; i < (int)mailsCount; i++){
            j = i - gap;
            while (j >= 0){
                k = j + gap;
                if (*mails[j] <= *mails[k]){
                    j = -1;
                }
                else{
                    std::swap(mails[j],mails[j+1]);
                    j -= gap;
                }
            }
        }
        gap = gap/2;
    }
}

long long MailSearcher::binarySearch(const char *senderSearched)
{
    long long i, j, middle, comparison;

    i = 0;
    j = mailsCount-1;
    while (i <= j){
        middle = (i + j)/2;
        comparison = strcmp(senderSearched,mails[middle]->getSender());
        if (comparison == 0){
            return middle;
        }
        else if (comparison < 0){
            j = middle - 1;
        }
        else{
            i = middle + 1;
        }
    }
    return -1;
}

std::stringstream MailSearcher::getMails(const char *senderSearched)
{
    long long i, j, middle, comparison;
    std::stringstream ss;

    middle = binarySearch(senderSearched);
    if (middle != -1){
        ss << *mails[middle];
        //Search concurrences on the left side
        if (middle > 0){
            i = middle - 1;
            j = 0;
            comparison = strcmp(mails[i]->getSender(),senderSearched);
            while (comparison == 0 and i >= j){
                ss << *mails[i];
                i--;
                comparison = strcmp(mails[i]->getSender(),senderSearched);
            }
        }
        //Search concurrences on the right side
        if (middle < mailsCount-1){
            i = middle + 1;
            j = mailsCount;
            comparison = strcmp(mails[i]->getSender(),senderSearched);
            while (comparison == 0 and i <= j){
                ss << *mails[i];
                i++;
                comparison = strcmp(mails[i]->getSender(),senderSearched);
            }
        }
    }
    return ss;
}


