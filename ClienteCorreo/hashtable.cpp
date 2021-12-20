#include "hashtable.h"

#include <QDebug>

//Index Methods

HashTable::Index::Index(){
    std::strcpy(this->key,"");
    this->value = -1;
}

HashTable::Index::Index(const char* key){
    std::strcpy(this->key,key);
    this->value = -1;
}

bool HashTable::Index::operator==(const Index &i) const{
    return std::strcmp(this->key,i.key) == 0;
}

bool HashTable::Index::operator!=(const Index& i) const{
    return std::strcmp(this->key,i.key) != 0;
}

bool HashTable::Index::operator<(const Index &i) const{
    return std::strcmp(this->key,i.key) < 0;
}

bool HashTable::Index::operator<=(const Index &i) const{
    return std::strcmp(this->key,i.key) <= 0;
}

//HashTable Methods

HashTable::HashTable(){
}

HashTable::~HashTable()
{
    deleteTable();
}

int HashTable::hashFunction(char* key){

    char* p;
    unsigned int h = 0, q;

    for (p = key; *p != '\0'; p++){
        h = (h << 4) + (*p);
        q = h & 0xF0000000;
        if (q > 1){
            h = h ^ (q >> 24);
            h = h ^ q;
        }
    }
    return (h % SIZE_TABLE);
}

bool HashTable::contains(char *key){
    int position;

    position = hashFunction(key);

    if (table[position].findData(Index(key))){
        return true;
    }

    return false;
}

void HashTable::add(Index& i){
    int position;
    position = hashFunction(i.key);
    table[position].insertData(table[position].getLastPos(),i);
    qDebug() << "Position = " << position;
    qDebug() << "Index = " << i.key << "," << i.value;
}

List<HashTable::Index> &HashTable::getIndexAt(char *key)
{
    int position;
    position = hashFunction(key);
    return table[position];
}

void HashTable::deleteTable()
{
    for (unsigned int i = 0; i < SIZE_TABLE; i++){
        if (!table[i].isEmpty()){
            table[i].deleteAll();
        }
    }
}

void HashTable::save(){
    std::fstream hashFile;
    hashFile.open(HASH_TABLE_FILE,std::ios::out | std::ios::binary);
    if (hashFile.is_open()){
        for (unsigned int i = 0 ; i < SIZE_TABLE; i++){
            List<Index>::Node* aux(table[i].getFirstPos());
            char size;
            size = table[i].getSize();
            hashFile.write(&size,sizeof(char));
            while (aux != nullptr){
                hashFile.write((char*)&aux->getData(),sizeof(Index));
                aux = aux->getNext();
            }
        }
        hashFile.close();
    }
}

void HashTable::load(){
    std::fstream hashFile;
    hashFile.open(HASH_TABLE_FILE,std::ios::in | std::ios::binary);
    if (hashFile.is_open()){
        for (unsigned int i = 0; i < SIZE_TABLE; i++){
            char size;
            hashFile.read(&size,sizeof(char));
            for (int j = 0; j < (int)size; j++){
                HashTable::Index index;
                hashFile.read((char*)&index,sizeof(HashTable::Index));
                qDebug() << "Load Index = " << index.key << "," << index.value;
                table[i].insertData(table[i].getLastPos(),index);
            }
        }
        hashFile.close();
    }
}
