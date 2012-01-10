#include "LoggableQuery.h"

LoggableQuery::LoggableQuery(Database& db) : Query(db)
{
}

bool LoggableQuery::executeAndLog(const std::string& sql)
{
    bool success = this->execute(sql);
    LOG(sql) << "Execute query: " + sql + " ( " + ( success ? "success" : "failed" ) + " ).";

    return success;
}

sqlite3_stmt* LoggableQuery::get_result_and_log(const std::string& sql)
{
    LOG(sql) << "Execute query: " + sql + ".";

    return this->get_result(sql);
}
