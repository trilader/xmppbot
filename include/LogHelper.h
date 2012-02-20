#ifndef LOGHELPER_H
#define LOGHELPER_H

#include "Log.h"
#include "ConsoleLog.h"
#include "FileLog.h"
#include "VoidLog.h"
#include "SQLiteLog.h"

#include "Configuration.h"

//macros:

#define LOG_INIT(c,n) LogHelper::init(c,#n);

#define LOG(n) (LogHelper::log(#n))

#define LOG_ADD(n,l) LogHelper::add(#n, l);

#define LOG_DELETE(n) LogHelper::remove(#n);

class LogHelper
{
    public:
        static void init(Configuration *config, const std::string& name);
        static void add(const std::string& name, Log *log);
        static Log& log(const std::string& name);
        static void remove(const std::string& name);

    private:
        static boost::unordered_map<std::string, Log* > logMap;
};

inline void LogHelper::init(Configuration *config, const std::string& name)
{
    bool enabled, keepopen;
    std::string type, fileformat, entryformat;
    bool exists = config->getLog(name,&type, &fileformat, &entryformat, &enabled, &keepopen);

    if(exists && enabled)
    {
            if("file" == type && "" != fileformat && "" != entryformat)
                LogHelper::logMap[name] = new FileLog(new StringFormat(fileformat),
                                                new StringFormat(entryformat), keepopen);
            else if("console" == type && "" != entryformat)
                LogHelper::logMap[name] = new ConsoleLog(new StringFormat(entryformat));
#if defined DB_SUPPORT && defined DB_SQLITE_SUPPORT
            else if("sql" == type && "" != fileformat)
                LogHelper::logMap[name] = new SQLiteLog(new StringFormat(fileformat), name);
#endif
    }

    if(!(LogHelper::logMap.count(name)))
        LogHelper::logMap[name] = new VoidLog();
}

inline void LogHelper::add(const std::string& name, Log *log)
{
    LogHelper::logMap[name] = log;
}

inline Log& LogHelper::log(const std::string& name)
{
    return (*(LogHelper::logMap[name]));
}

inline void LogHelper::remove(const std::string& name)
{
    if(!LogHelper::logMap.count(name))
        return;
    Log *dummy = LogHelper::logMap[name];
    LogHelper::logMap.erase(name);

    delete dummy;
}

#endif
