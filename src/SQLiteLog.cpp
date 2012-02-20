#include "SQLiteLog.h"

SQLiteLog::SQLiteLog(StringFormat *databaseFormat, std::string table) : DbLog(soci::sqlite3, databaseFormat, table)
{
}
