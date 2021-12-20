#include "mail.h"

#include <QDebug>

Mail::Mail()
{
    std::chrono::system_clock::time_point today(std::chrono::system_clock::now());
    time_t tt;
    tt = std::chrono::system_clock::to_time_t(today);
    struct tm* timeInfo;
    timeInfo = localtime(&tt);
    sprintf(date,"%02i/%02i/%02i",timeInfo->tm_year+1900,timeInfo->tm_mon+1,timeInfo->tm_mday);
    sprintf(time,"%02i:%02i",timeInfo->tm_hour,timeInfo->tm_min);
    this->id = 0;
    std::strcpy(this->sender,"");
    std::strcpy(this->addressee,"");
    std::strcpy(this->cc,"");
    std::strcpy(this->cco,"");
    std::strcpy(this->affair,"");
    std::strcpy(this->content,"");
}

Mail::~Mail()
{

}

bool Mail::setId(const unsigned int id)
{
    if (id < MAX_ID ){
        this->id = id;
        return true;
    }
    return false;
}

bool Mail::setDate(const char *date)
{
    if (strlen(date) <= DATE_SIZE+1){
        strcpy(this->date,date);
        return true;
    }
    return false;
}

bool Mail::setTime(const char *time)
{
    if (strlen(time) <= TIME_SIZE+1){
        strcpy(this->time,time);
        return true;
    }
    return false;
}

bool Mail::setSender(const char *sender)
{
    if (strlen(sender) > 0  and strlen(sender) <= MAIL_SIZE){
        strcpy(this->sender,sender);
        return true;
    }
    return false;
}

bool Mail::setAddressee(const char *addressee)
{
    if (strlen(addressee) > 0 and strlen(addressee) <= MAIL_SIZE){
        strcpy(this->addressee,addressee);
        return true;
    }
    return false;
}

bool Mail::setCc(const char *cc)
{
    if (strlen(cc) > 0 and strlen(cc) <= MAIL_SIZE){
        strcpy(this->cc,cc);
        return true;
    }
    return false;
}

bool Mail::setCco(const char *cco)
{
    if (strlen(cco) > 0 and strlen(cc) <= MAIL_SIZE){
        strcpy(this->cco,cco);
        return true;
    }
    return false;
}

bool Mail::setAffair(const char *affair)
{
    if (strlen(affair) > 0 and strlen(affair) <= AFFAIR_SIZE){
        strcpy(this->affair,affair);
        return true;
    }
    return false;
}

bool Mail::setContent(const char *content)
{
    if (strlen(content) > 0 and strlen(content) <= CONTENT_SIZE){
        strcpy(this->content,content);
        return true;
    }
    return false;
}

unsigned int Mail::getId()
{
    return id;
}

char* Mail::getDate()
{
    return date;
}

char* Mail::getTime()
{
    return time;
}

char* Mail::getSender()
{
    return sender;
}

char* Mail::getAddressee()
{
    return addressee;
}

char* Mail::getCc()
{
    return cc;
}

char* Mail::getCco()
{
    return cco;
}

char* Mail::getAffair()
{
    return affair;
}

char* Mail::getContent()
{
    return content;
}

std::string Mail::toString()
{
    std::stringstream ss;

    ss << *this;

    return ss.str();
}

//Operators

Mail &Mail::operator =(Mail &m)
{
    this->id = m.id;
    setDate(m.date);
    setTime(m.time);
    strcpy(this->sender,m.sender);
    strcpy(this->addressee,m.addressee);
    strcpy(this->cc,m.cc);
    strcpy(this->cco,m.cco);
    strcpy(this->affair,m.affair);
    strcpy(this->content,m.content);
    return *this;
}

bool Mail::operator <(const Mail &m) const
{
    return (std::strcmp(sender,m.sender) < 0);
}

bool Mail::operator <=(const Mail& m) const
{
    return (std::strcmp(sender,m.sender) <= 0);
}

bool Mail::operator >(const Mail &m) const
{
    return (std::strcmp(sender,m.sender) > 0);
}

bool Mail::operator >=(const Mail &m) const
{
    return (std::strcmp(sender,m.sender) >= 0);
}

bool Mail::operator ==(const Mail &m) const
{
    return (std::strcmp(sender,m.sender) == 0);
}

std::istream& operator>> (std::istream& i,Mail& m){
    std::string str;
    std::getline(i,str,',');
    m.id = atol(str.c_str());
    std::getline(i,str,',');
    m.setDate(str.c_str());
    std::getline(i,str,',');
    m.setTime(str.c_str());
    std::getline(i,str,',');
    m.setSender(str.c_str());
    std::getline(i,str,',');
    m.setAddressee(str.c_str());
    std::getline(i,str,',');
    m.setCc(str.c_str());
    std::getline(i,str,',');
    m.setCco(str.c_str());
    std::getline(i,str,',');
    m.setAffair(str.c_str());
    std::getline(i,str,'\r');
    i.get();
    m.setContent(str.c_str());
    return i;
}

std::ostream& operator<< (std::ostream& o,Mail& m){
    o << m.id << ",";
    o << m.date << ",";
    o << m.time << ",";
    o << m.sender << ",";
    o << m.addressee << ",";
    o << m.cc << ",";
    o << m.cco << ",";
    o << m.affair << ",";
    o << m.content << CRLF;
    return o;
}


