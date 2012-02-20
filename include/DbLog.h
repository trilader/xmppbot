#ifndef DBLOG_H
#define DBLOG_H

#ifdef DB_SUPPORT

#include "Log.h"
#include "StringFormat.h"
#include "DatabaseHelper.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"

class DbLog : public Log
{
public:
    virtual void log(const std::string& msg);

protected:

    DbLog(const soci::backend_factory& factory, StringFormat *databaseFormat, std::string table);

    virtual void openDatabase();

    soci::session *_db;
    const soci::backend_factory& _factory;

    StringFormat *_databaseFormat;
    StringFormat *_tableFormat;
    StringFormat *_insertQueryFormat;
    StringFormat *_createQueryFormat;

    boost::gregorian::date *_today;
    long _thishour;
};

#endif

#endif
