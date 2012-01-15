#ifndef DATABASEHELPER_H
#define DATABASEHELPER_H

#include "sqlite3.h"

#ifndef WIN32

#include "libsqlitewrapped.h"

#else //WIN32

#include "Database.h"
#include "Query.h"
#include "IError.h"
#include "StderrLog.h"
#include "SysLog.h"

#endif //WIN32

#endif
