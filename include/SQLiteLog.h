#ifndef SQLITELOG_H
#define SQLITELOG_H

#if defined DB_SUPPORT && defined DB_SQLITE_SUPPORT

#include "DbLog.h"

class SQLiteLog : public DbLog
{
public:
    SQLiteLog(StringFormat *databaseFormat, std::string table);
};

#endif

#endif
