#ifndef SQLITELOG_H
#define SQLITELOG_H

#include "DbLog.h"

class SQLiteLog : public DbLog
{
public:
    SQLiteLog(StringFormat *databaseFormat, std::string table);
};

#endif
