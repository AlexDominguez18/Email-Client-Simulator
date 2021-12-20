#ifndef PRIMARYINDEX_H
#define PRIMARYINDEX_H

#include <iostream>
#include <cstring>

#define KEY_SIZE 10


class PrimaryIndex
{
private:

    unsigned int value;
    char key[KEY_SIZE+1];

public:
    PrimaryIndex();
    PrimaryIndex(const char* k);

    void setValue(unsigned int value);
    void setKey(const char* key);

    unsigned int getValue();
    const char* getKey();

    //PrimaryIndex& operator = (const PrimaryIndex& i);

    bool operator == (const PrimaryIndex& i) const;
    bool operator <= (const PrimaryIndex& i) const;
    bool operator < (const PrimaryIndex& i) const;
    bool operator >= (const PrimaryIndex& i) const;
    bool operator > (const PrimaryIndex& i) const;

    friend std::ostream& operator << (std::ostream& o, PrimaryIndex& i);
};

#endif // PRIMARYINDEX_H
