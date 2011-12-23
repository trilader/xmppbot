#ifndef SQLITELOG_H
#define SQLITELOG_H

#include "Log.h"
#include "StringFormat.h"
#include "DatabaseHelper.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"

class SQLiteLog : public Log
{
public:
    SQLiteLog(std::string database, std::string table, bool keepopen = false);
    void log(const std::string& msg);

private:
    void openDatabase();

    bool _keepopen;
    Database *_db;

    std::string _table;
};

#endif

