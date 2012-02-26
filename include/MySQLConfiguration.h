#ifndef MYSQLCONFIGURATION_H
#define MYSQLCONFIGURATION_H

#if defined DB_SUPPORT && defined DB_MYSQL_SUPPORT

#include "DbConfiguration.h"

class MySQLConfiguration : public DbConfiguration
{
    public:
        MySQLConfiguration(std::string connString, bool writeable = true, unsigned int index = 0);

    protected:

        virtual void initTables();
};

#endif

#endif

