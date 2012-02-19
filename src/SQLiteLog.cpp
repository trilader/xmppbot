#include "SQLiteLog.h"

SQLiteLog::SQLiteLog(StringFormat *databaseFormat, std::string table)
{
    this->_databaseFormat = databaseFormat;

    this->_tableFormat = new StringFormat("xmppbot__%1_log");
    this->_tableFormat->assign("1", table);
    this->_tableFormat->produce();

    this->_createQueryFormat = new StringFormat(std::string("CREATE TABLE IF NOT EXISTS %table ( ")
                                                      +"%l_id integer primary key autoincrement not null,"
                                                      +"%l_entry varchar(255) default '' not null,"
                                                      +"%l_assoc_user varchar(255) default '' not null,"
                                                      +"%l_timestamp integer not null);");
    this->_createQueryFormat->assign("l", table);

    this->_insertQueryFormat = new StringFormat("INSERT INTO %table ( %l_entry, %l_assoc_user, %l_timestamp ) VALUES (:msg,'',datetime('now'))");
    this->_insertQueryFormat->assign("l", table);

    this->_today = new boost::gregorian::date(boost::gregorian::min_date_time);
    this->_thishour = boost::posix_time::second_clock::local_time().time_of_day().hours();

    this->_db = 0;
    this->openDatabase(); //do this to make sure we can open the db at startup
}

void SQLiteLog::openDatabase()
{
    boost::gregorian::date today = boost::gregorian::day_clock::local_day();
    boost::posix_time::ptime t = boost::posix_time::second_clock::local_time();

    if((0 != this->_db) && (today == (*this->_today)) && (t.time_of_day().hours() == (this->_thishour)))
        return;

    *this->_today = today;
    this->_thishour = t.time_of_day().hours();

    if(0 == this->_db)
        delete (this->_db);

    this->assignFormatDateTime(this->_databaseFormat);
    this->_db = new soci::session(soci::sqlite3, this->_databaseFormat->produce());

    this->_createQueryFormat->assign("table", this->_tableFormat->last());
    *this->_db << this->_createQueryFormat->produce();
}

void SQLiteLog::log(const std::string& msg)
{
    this->openDatabase();

    this->_insertQueryFormat->assign("table", this->_tableFormat->last());

    *this->_db << this->_insertQueryFormat->produce(), soci::use(msg, "msg");
}
