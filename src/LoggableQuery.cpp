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
