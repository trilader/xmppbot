#include "SQLiteLog.h"

#if defined DB_SUPPORT && defined DB_SQLITE_SUPPORT

SQLiteLog::SQLiteLog(StringFormat *databaseFormat, std::string table) : DbLog(soci::sqlite3, databaseFormat, table)
{
}

#endif
