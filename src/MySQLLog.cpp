#include "MySQLLog.h"

#if defined DB_SUPPORT && defined DB_MYSQL_SUPPORT

MySQLLog::MySQLLog(StringFormat *databaseFormat, std::string table) : DbLog(soci::mysql, databaseFormat, table)
{
}

#endif
