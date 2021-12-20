#ifndef SECONDARYINDEX_H
#define SECONDARYINDEX_H

#include <iostream>
#include <cstring>

#include "list.h"

#define SECONDARY_KEY_SIZE 100
#define VALUE_SIZE 10

class SecondaryIndex
{
    char key[SECONDARY_KEY_SIZE+1];
    List<unsigned int> values;

public:
    SecondaryIndex();
    SecondaryIndex(const char* key);

    void setKey(const char* key);
    void insertValue(unsigned int value);

    char *getKey();
    List<unsigned int> &getValues();

    //SecondaryIndex& operator = (SecondaryIndex& si);

    bool operator == (const SecondaryIndex& si) const;
    bool operator <= (const SecondaryIndex& si) const;
    bool operator < (const SecondaryIndex& si) const;
    bool operator >= (const SecondaryIndex& si) const;
    bool operator > (const SecondaryIndex& si) const;

    friend std::ostream& operator << (std::ostream& o, SecondaryIndex& i);
};

#endif // SECONDARYINDEX_H
