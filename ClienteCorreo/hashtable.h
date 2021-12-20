#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <cstring>
#include <fstream>

#include "list.h"

#define SIZE_TABLE 1009
#define KEY_SIZE_INDEX 30
#define HASH_TABLE_FILE "../Files/hashtable.dat"

class HashTable{
public:
    class Index{
        public:
            char key[KEY_SIZE_INDEX+1];
            unsigned int value;
            Index();
            Index(const char* key);
            bool operator == (const Index& i) const;
            bool operator != (const Index& i) const;
            bool operator < (const Index& i) const;
            bool operator <= (const Index& i) const;
    };
private:

    List<Index> table[SIZE_TABLE];
    int hashFunction(char* key);

public:
    HashTable();
    ~HashTable();

    bool contains(char* key);
    void add(Index& i);
    List<Index>& getIndexAt(char* key);
    void deleteTable();
    void save();
    void load();
};

#endif // HASHTABLE_H
