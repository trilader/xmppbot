#ifndef SQLITECONFIGURATION_H
#define SQLITECONFIGURATION_H

#include "DbConfiguration.h"

class SQLiteConfiguration : public DbConfiguration
{
    public:
        SQLiteConfiguration(std::string filename, bool writeable = true, unsigned int index = 0);

    protected:

        virtual void initTables();
};

#endif
