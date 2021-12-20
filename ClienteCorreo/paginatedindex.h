#ifndef PAGINATEDINDEX_H
#define PAGINATEDINDEX_H

#include <QDateTime>

#include "primaryindex.h"

class PaginatedIndex : public PrimaryIndex
{
private:
    QDateTime timestamp;
public:
    PaginatedIndex();
    PaginatedIndex(const char* key);

    void setDateTime(QDateTime& t);
    QDateTime getDateTime();

    bool cmpDateTime(QDateTime& t);
};

#endif // PAGINATEDINDEX_H
