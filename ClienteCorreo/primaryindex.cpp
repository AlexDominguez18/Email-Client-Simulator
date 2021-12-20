#include "primaryindex.h"

PrimaryIndex::PrimaryIndex()
{
    std::strcpy(this->key,"0");
    this->value = 0;
}

PrimaryIndex::PrimaryIndex(const char* k)
{
    std::strcpy(this->key,k);
    this->value = -1;
}

void PrimaryIndex::setValue(unsigned int value)
{
    this->value = value;
}

void PrimaryIndex::setKey(const char *key)
{
    std::strcpy(this->key,key);
}

unsigned int PrimaryIndex::getValue()
{
    return value;
}

const char* PrimaryIndex::getKey()
{
    return key;
}

/*PrimaryIndex& PrimaryIndex::operator = (const PrimaryIndex& i)
{
    value = i.value;
    std::strcpy(key,i.key);
    return *this;
}*/

bool PrimaryIndex::operator == (const PrimaryIndex& i) const
{
    return std::strcmp(key,i.key) == 0;
}

bool PrimaryIndex::operator <= (const PrimaryIndex& i) const
{
    return std::strcmp(key,i.key) <= 0;
}

bool PrimaryIndex::operator < (const PrimaryIndex& i) const
{
    return std::strcmp(key,i.key) < 0;
}

bool PrimaryIndex::operator >= (const PrimaryIndex& i) const
{
    return std::strcmp(key,i.key) >= 0;
}

bool PrimaryIndex::operator > (const PrimaryIndex& i) const
{
    return std::strcmp(key,i.key) > 0;
}

std::ostream& operator << (std::ostream& o, PrimaryIndex& i)
{
    o << i.key << ",";
    o << i.value << "\r\n";
    return o;
}
