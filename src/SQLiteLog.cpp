#include "SQLiteLog.h"

SQLiteLog::SQLiteLog(std::string database, std::string table, bool keep_open)
{
    this->_db = new Database(database);
    this->_keepopen = keep_open;
    this->_table = table;

    Query query(*(this->_db));
    query.execute("IF NOT TABLE EXISTS " + table + " CREATE TABLE " + table + " ( id integer primary key autoincrement not null, entry varchar(255) default '' not null );");
}

void SQLiteLog::openDatabase()
{

}

void SQLiteLog::log(const std::string& msg)
{
    Query query(*(this->_db));
    query.execute("INSERT INTO " + this->_table + " VALUES ('" + msg +" ')");
}
