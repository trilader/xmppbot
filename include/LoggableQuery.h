#ifndef LOGGABLEQUERY_H
#define LOGGABLEQUERY_H

#include "DatabaseHelper.h"
#include "LogHelper.h"

class LoggableQuery
{
    public:
        LoggableQuery(soci::session& db);
        bool executeAndLog(const std::string& sql);
        std::string get_result_and_log(const std::string& sql);
};

#endif
