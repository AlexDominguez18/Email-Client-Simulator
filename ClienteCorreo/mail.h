#ifndef MAIL_H
#define MAIL_H

#include <ctime>
#include <chrono>
#include <cstring>
#include <string>
#include <iostream>
#include <stdio.h>

#include "csvfile.h"

#define MAX_ID 999999999
#define DATE_SIZE 10
#define TIME_SIZE 5
#define MAIL_SIZE 100
#define AFFAIR_SIZE 30
#define CONTENT_SIZE 140

class Mail
{
private:
    unsigned int id;
    char date[DATE_SIZE+1];
    char time[TIME_SIZE+1];
    char sender[MAIL_SIZE+1];
    char addressee[MAIL_SIZE+1];
    char cc[MAIL_SIZE+1];
    char cco[MAIL_SIZE+1];
    char affair[AFFAIR_SIZE+1];
    char content[CONTENT_SIZE+1];
public:
    Mail();
    ~Mail();

    //Setters
    bool setId(const unsigned int id);
    bool setDate(const char* date);
    bool setTime(const char* time);
    bool setSender(const char* sender);
    bool setAddressee(const char* addressee);
    bool setCc(const char* cc);
    bool setCco(const char* cco);
    bool setAffair(const char* affair);
    bool setContent(const char* content);

    //Getters
    unsigned int getId();
    char* getDate();
    char* getTime();
    char* getSender();
    char* getAddressee();
    char* getCc();
    char* getCco();
    char* getAffair();
    char* getContent();
    std::string toString();

    //Operators
    Mail& operator = (Mail& m);
    bool operator <(const Mail& m) const;
    bool operator <=(const Mail& m) const;
    bool operator >(const Mail& m) const;
    bool operator >=(const Mail& m) const;
    bool operator ==(const Mail& m) const;
    friend std::istream& operator>> (std::istream& i,Mail& m);
    friend std::ostream& operator<< (std::ostream& i,Mail& m);
};

#endif // MAIL_H
