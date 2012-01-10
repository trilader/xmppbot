#ifndef SQLITELOG_H
#define SQLITELOG_H

#include "Log.h"
#include "SQLiteStringFormat.h"
#include "DatabaseHelper.h"
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
    SQLiteStringFormat *_insertQueryFormat;
    SQLiteStringFormat *_createQueryFormat;

    boost::gregorian::date *_today;
    long _thishour;
};

#endif

