#include "secondaryindex.h"

#include <QDebug>

SecondaryIndex::SecondaryIndex()
{

}

SecondaryIndex::SecondaryIndex(const char *key)
{
    strcpy(this->key,key);
}

void SecondaryIndex::setKey(const char *key)
{
    strcpy(this->key,key);
}

void SecondaryIndex::insertValue(unsigned int value)
{
    values.insertData(values.getLastPos(),value);
}

char *SecondaryIndex::getKey()
{
    return key;
}

List<unsigned int> &SecondaryIndex::getValues()
{
    return values;
}

/*SecondaryIndex &SecondaryIndex::operator =(SecondaryIndex &si)
{
    strcpy(this->key,si.key);
    values = si.values;

    return *this;
}*/

bool SecondaryIndex::operator == (const SecondaryIndex &si) const
{
    return strcmp(key,si.key) == 0;
}

bool SecondaryIndex::operator <= (const SecondaryIndex &si) const
{
    return strcmp(key,si.key) <= 0;
}

bool SecondaryIndex::operator < (const SecondaryIndex &si) const
{
    return strcmp(key,si.key) < 0;
}

bool SecondaryIndex::operator >= (const SecondaryIndex &si) const
{
    return strcmp(key,si.key) >= 0;
}

bool SecondaryIndex::operator > (const SecondaryIndex &si) const
{
    return strcmp(key,si.key) > 0;
}

std::ostream& operator << (std::ostream& o, SecondaryIndex& i)
{
    o << i.key << "\r\n";
    return o;
}


