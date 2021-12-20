#include "paginatedindex.h"

PaginatedIndex::PaginatedIndex()
{

}

PaginatedIndex::PaginatedIndex(const char *key)
{
    this->setKey(key);
}

void PaginatedIndex::setDateTime(QDateTime& t)
{
    this->timestamp = t;
}

QDateTime PaginatedIndex::getDateTime()
{
    return timestamp;
}

bool PaginatedIndex::cmpDateTime(QDateTime& t)
{
    return this->timestamp < t;
}


