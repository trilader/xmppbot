#include "SQLiteLog.h"

SQLiteLog::SQLiteLog(StringFormat *databaseFormat, std::string table)
{
    this->_databaseFormat = databaseFormat;

    this->_tableFormat = new StringFormat("xmppbot__%1_log");
    this->_tableFormat->assign("1", table);
    this->_tableFormat->produce();

    this->_createQueryFormat = new StringFormat("CREATE TABLE IF NOT EXISTS %table ( id integer primary key autoincrement not null, entry varchar(255) default '' not null );");

    this->_insertQueryFormat = new StringFormat("INSERT INTO %table ( entry ) VALUES ('%msg')");

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
    this->_db = new Database(this->_databaseFormat->produce());

    LoggableQuery query(*(this->_db));

    this->_createQueryFormat->assign("table", this->_tableFormat->last());
    query.executeAndLog(this->_createQueryFormat->produce());
}

void SQLiteLog::log(const std::string& msg)
{
    this->openDatabase();

    LoggableQuery query(*(this->_db));

    this->_insertQueryFormat->assign("table", this->_tableFormat->last());
    this->_insertQueryFormat->assign("msg", msg);
    query.executeAndLog(this->_insertQueryFormat->produce());
}
