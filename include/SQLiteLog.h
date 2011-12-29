#ifndef SQLITELOG_H
#define SQLITELOG_H

#include "Log.h"
#include "StringFormat.h"
#include "DatabaseHelper.h"
#include "LoggableQuery.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"

class SQLiteLog : public Log
{
public:
    SQLiteLog(StringFormat *databaseFormat, std::string table);
    void log(const std::string& msg);

private:
    void openDatabase();

    Database *_db;

    StringFormat *_databaseFormat;
    StringFormat *_tableFormat;
    StringFormat *_insertQueryFormat;
    StringFormat *_createQueryFormat;

    boost::gregorian::date *_today;
    long _thishour;
};

#endif

