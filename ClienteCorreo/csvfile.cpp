#include "csvfile.h"

#include <QDebug>

CSVFile::CSVFile(){ }

std::string CSVFile::toCsv(std::string &field)
{
    int index;
    std::string subStr = DOUBLE_QUOTES;

    //Addign CR
    index = field.find_first_of(ENDL);
    if (index != (int)std::string::npos){
        while (index != (int)std::string::npos){
            field.replace(index,1,CRLF);
            index = field.find_first_of(ENDL,index+2);
        }
    }
    //Replacing the words with quotes
    index = field.find_first_of(QUOTES);
    if (index != (int)std::string::npos){
        while(index != (int)std::string::npos){
            field.replace(index,1,subStr);
            index = field.find_first_of(QUOTES,index+subStr.length());
        }
        field.replace(0,0,"\"");
        field.replace(field.length(),1,"\"");
        return field;
    }
    //Adding quotes to the field if this has a comma
    index = field.find(DELIMITER);
    if (index != (int)std::string::npos){
        field.replace(0,0,"\"");
        field.replace(field.length(),1,"\"");
        return field;
    }
    //Adding quotes to the field if this has a endl
    index = field.find(ENDL);
    if (index != (int)std::string::npos){
        field.replace(0,0,"\"");
        field.replace(field.length(),1,"\"");
    }
    return field;
}

std::string CSVFile::parseCsvRecord(std::string &record)
{
    int state = UNQUOTED_FIELD;
    int fieldsCount = 1,index = 0;
    char character;
    std::string* fixedRecord = new std::string[fieldsCount];
    std::stringstream ss;

    for (int i = 0; i < (int)record.length();i++){
        character = record[i];
        switch(state){
            case UNQUOTED_FIELD:
                switch (character) {
                    case DELIMITER:
                        addField(fieldsCount++,fixedRecord);
                        fixedRecord[fieldsCount-1] = "";
                        index++;
                        break;
                    case QUOTES:
                        state = QUOTED_FIELD;
                        break;
                    default:
                        fixedRecord[index] += character;
                }
                break;
            case QUOTED_FIELD:
                switch (character){
                    case QUOTES:
                        state = QUOTEDQUOTE;
                        break;
                    default:
                        fixedRecord[index] += character;
                }
                break;
            case QUOTEDQUOTE:
                switch (character) {
                    case DELIMITER:
                        addField(fieldsCount++,fixedRecord);
                        fixedRecord[fieldsCount-1] = "";
                        index++;
                        state = UNQUOTED_FIELD;
                        break;
                    case QUOTES:
                        fixedRecord[index] += QUOTES;
                        state = QUOTED_FIELD;
                        break;
                    default:
                        state = UNQUOTED_FIELD;
                }
                break;
        }
    }
    fieldsCount--;
    for (int i = 0; i < fieldsCount;i++){
        ss << fixedRecord[i] << DELIMITER;
    }
    ss << fixedRecord[fieldsCount] << CRLF;
    return ss.str();
}

std::stringstream CSVFile::parseRecords()
{
    std::string str,record = "";
    std::stringstream ss;
    int end;

    file.open(CSV_FILE,std::ios::in);
    if (file.is_open()){
        //Get final position of the file
        file.seekg(0,std::ios_base::end);
        end = file.tellg();
        file.seekg(0,std::ios_base::beg);
        //Removing headers
        getline(file,str,'\r');
        file.ignore();
        getline(file,str,'\r');
        file.ignore();
        while (file.tellg() < end) {
            ss << getNextToken();
            qDebug() << file.tellg() << endl;
        }
        file.close();
    }
    else{
        qDebug() << "No se abrio el archivo en parseRecords\n";
    }
    return ss;
}

std::string CSVFile::getNextToken()
{
    int state = UNQUOTED_FIELD;
    int pos = file.tellg();
    bool readFile = true;
    char character;
    std::string fixedField = "";

    while (readFile){
        file.seekg(pos++,std::ios::beg);
        file.read(&character,1);
        switch(state){
            case UNQUOTED_FIELD:
                if (character == QUOTES){
                    state = QUOTED_FIELD;
                }
                else if (character == DELIMITER){
                    readFile = false;
                    fixedField += DELIMITER;
                }
                else if (character == '\r' or character == '\n'){
                    readFile = false;
                    fixedField += character;
                }
                else{
                    fixedField += character;
                }
                break;
            case QUOTED_FIELD:
                if (character == QUOTES){
                    state = QUOTEDQUOTE;
                }
                else if (character == '\r'){
                    break;
                }
                else{
                    fixedField += character;
                }
                break;
            case QUOTEDQUOTE:
                if (character == QUOTES){
                    fixedField += QUOTES;
                    state = QUOTED_FIELD;
                }
                else if (character == DELIMITER){
                    readFile = false;
                    fixedField += DELIMITER;
                }
                else if (character == '\r' or character == '\n'){
                    readFile = false;
                    fixedField += character;
                }
                else{
                    state = UNQUOTED_FIELD;
                }
                break;
        }
    }
    return fixedField;
}

void CSVFile::addField(int counter, std::string *&record)
{
    std::string* tmp = new std::string[counter+1];
    for (int j = 0; j < counter;j++){
        tmp[j] = record[j];
    }
    delete[] record;
    record = tmp;
}

void CSVFile::write(unsigned int counter,std::string content)
{
    file.open(CSV_FILE,std::ios::out);
    if (file.is_open()){
        file << counter << CRLF;
        file << HEADER << CRLF;
        file << content;
    }
    file.close();
}

void CSVFile::update(unsigned int pos, std::string &content)
{
    unsigned int counter,i;
    std::fstream tmp;
    std::string str;
    file.open(CSV_FILE,std::ios::in);
    tmp.open(TMP_FILE,std::ios::out);
    if (tmp.is_open()){
        //Remove counter and header
        getline(file,str,'\r');
        file.ignore();
        counter = atol(str.c_str());
        getline(file,str,'\r');
        file.ignore();
        tmp << counter << CRLF;
        tmp << HEADER << CRLF;
        for (i = 0;i < pos;i++){
            getline(file,str,'\r');
            file.ignore();
            tmp << str << CRLF;
        }
        getline(file,str,'\r');
        file.ignore();
        qDebug() << "CONTENT = " << content.c_str() << endl;
        tmp << content;
        for (i = pos;i < counter;i++){
            getline(file,str,'\r');
            file.ignore();
            tmp << str << CRLF;
        }
        tmp.close();
        file.close();
        remove(CSV_FILE);
        rename(TMP_FILE,CSV_FILE);
    }
    else{
        qDebug() << "NO SE ABRIO";
    }
}

void CSVFile::deleteRecord(unsigned int pos)
{
    unsigned int counter,newCounter,i;
    std::fstream tmp;
    std::string str;
    file.open(CSV_FILE,std::ios::in);
    tmp.open(TMP_FILE,std::ios::out);
    if (tmp.is_open()){
        //Remove counter and header
        getline(file,str,'\r');
        file.ignore();
        counter = atol(str.c_str());
        getline(file,str,'\r');
        file.ignore();
        newCounter = counter-1;
        tmp << newCounter << CRLF;
        tmp << HEADER << CRLF;
        for (i = 0;i < counter;i++){
            getline(file,str,'\r');
            file.ignore();
            if (i != pos){
                tmp << str << CRLF;
            }
        }
        tmp.close();
        file.close();
        remove(CSV_FILE);
        rename(TMP_FILE,CSV_FILE);
    }
    else{
        qDebug() << "NO SE ABRIO" << endl;
    }
}

bool CSVFile::haveData()
{
    file.open(CSV_FILE);
    file.seekg(0,std::ios::end);
    if (file.tellg() != -1){
        file.close();
        return true;
    }
    file.close();
    return false;
}

std::string CSVFile::encrypt(std::string str)
{
    std::string ciphered = "";

    for (int i = 0; i < (int)str.length(); i++){
        ciphered += cipherCaesar(str[i]);
    }

    return ciphered;
}

std::string CSVFile::desencrypt(std::string str)
{
    std::string deciphered = "";

    for (int i = 0; i < (int)str.length(); i++){
        deciphered += decipherCaesar(str[i]);
    }

    return deciphered;
}

char CSVFile::cipherCaesar(char c)
{
    const int displacement = 3;

    if (((int)c + displacement) > 255){
        return (((int)c + displacement) - 255);
    }
    else{
        return (int)c + displacement;
    }
}

char CSVFile::decipherCaesar(char c)
{
    const int displacement = 3;

    if (((int)c - displacement) < 0){
        return (((int)c - displacement) + 255);
    }
    else{
        return ((int)c - displacement);
    }
}
