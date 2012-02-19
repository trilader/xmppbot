#include "LoggableQuery.h"

LoggableQuery::LoggableQuery(soci::session& db)
{
}

bool LoggableQuery::executeAndLog(const std::string& sql)
{
    //bool success = this->execute(sql);
    LOG(sql) << "Execute query: " + sql + " ( " + ( true ? "success" : "failed" ) + " ).";

    return true;
}

std::string LoggableQuery::get_result_and_log(const std::string& sql)
{
    LOG(sql) << "Execute query: " + sql + ".";

    return sql;
}
