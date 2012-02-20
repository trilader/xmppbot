#ifndef DATABASEHELPER_H
#define DATABASEHELPER_H

#ifdef DB_SUPPORT

    #include "soci.h"

    #ifdef DB_SQLITE_SUPPORT
    #include "sqlite3/soci-sqlite3.h"
    #endif

    #ifdef DB_MYSQL_SUPPORT
    #include "mysql/soci-mysql.h"
    #endif

#endif

#endif
