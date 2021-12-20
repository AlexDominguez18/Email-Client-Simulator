#ifndef CSVFILE_H
#define CSVFILE_H

#include <fstream>
#include <iostream>
#include <sstream>

#define DELIMITER ','
#define QUOTES '\"'
#define DOUBLE_QUOTES "\"\""
#define ENDL "\n"
#define HEADER "ID,FECHA,HORA,REMITEMTE,DESTINATARIO,CC,CCO,ASUNTO,CONTENIDO"
#define CRLF "\r\n"
#define CSV_FILE "../Files/mails.csv"
#define TMP_FILE "../Files/tmp.csv"

enum Fields
{
    ID,DATE,TIME,SENDER,ADDRESSEES,AFFAIR,CONTENT
};

enum States
{
    UNQUOTED_FIELD,QUOTED_FIELD,QUOTEDQUOTE
};

class CSVFile
{
public:
    std::fstream file;
    CSVFile();
    std::string toCsv(std::string& field);
    std::string parseCsvRecord(std::string& record);
    std::stringstream parseRecords();
    std::string getNextToken();
    void addField(int counter,std::string*& record);
    void write(unsigned int counter,std::string content);
    void update(unsigned int id,std::string& content);
    void deleteRecord(unsigned int pos);
    bool haveData();
    std::string encrypt(std::string str);
    std::string desencrypt(std::string str);
    char cipherCaesar(char c);
    char decipherCaesar(char c);
};

#endif // CSVFILE_H
