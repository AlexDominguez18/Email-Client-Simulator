#ifndef MAILINDEX_H
#define MAILINDEX_H

#include <iostream>
#include <chrono>
#include <fstream>

#include "mail.h"
#include "maildao.h"
#include "primaryindex.h"
#include "secondaryindex.h"
#include "avltree.h"
#include "list.h"
#include "hashtable.h"

#define PRIMARY_INDEX_FILE "../Files/primaryIndex.dat"
#define TMP_PRIMARY_INDEX "../Files/tmpindex.dat"
#define AND_BOOLEAN "AND"
#define OR_BOOLEAN "OR"
#define MAX_NODES 10
#define LOAD_HASH_FILE 1
#define LOAD_MAILS_FILE 0

class MailIndex
{
private:

    MailDao mailDao;
    List<Mail> mails;

    //Primary Index
    std::fstream piFile;
    AVLTree<PrimaryIndex> index;

    void loadFilePi();
    void loadFileMails();

    //Secondary Index
    AVLTree<SecondaryIndex>* indexSender;
    AVLTree<SecondaryIndex>* indexAddressee;

    //HashTable
    HashTable* hashMails;

public:

    MailIndex();
    ~MailIndex();

    void insertPrimaryIndex(PrimaryIndex& pi);
    void deletePrimaryIndex(PrimaryIndex& pi);
    void savePrimaryIndex();
    void savePaginatedIndex();
    void primaryIndexUpdate(bool updated);
    Mail* findMail(const char* key);
    List<Mail> &getMailsSender(const char* key);
    List<Mail> &getMailsAddressee(const char* key);
    List<Mail> &getMailsBoolean(std::string text);
    unsigned int getPiCount();
    long long getCurrentTimeMilliseconds();
    void loadFileMailsSi();
    void deleteSecondaryIndex();

    //Delete HashTable
    void loadHashTable(int loadMode);
    void saveHashTable();
    List<Mail> &findHashMail(char* key);
    void deleteHashTable();
};

#endif // MAILINDEX_H
