#ifndef MYSQLLOG_H
#define MYSQLLOG_H

#if defined DB_SUPPORT && defined DB_MYSQL_SUPPORT

#include "DbLog.h"

class MySQLLog : public DbLog
{
public:
    MySQLLog(StringFormat *databaseFormat, std::string table);
};

#endif

#endif

