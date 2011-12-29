#ifndef LOGGABLEQUERY_H
#define LOGGABLEQUERY_H

#include "DatabaseHelper.h"
#include "LogHelper.h"

class LoggableQuery : public Query
{
    public:
        LoggableQuery(Database& db);
        bool executeAndLog(const std::string& sql);
};

#endif
