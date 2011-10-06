#ifndef LOGHELPER_H
#define LOGHELPER_H

#include "Log.h"
#include "ConsoleLog.h"
#include "FileLog.h"
#include "VoidLog.h"

//macros:
#define LOG_CONFIG(n) ("logs."#n".enabled", "(yes|no) Enable or disable the log") \
                        ("logs."#n".type", "(console|file) The type of the log") \
                        ("logs."#n".entryformat", "Custom format string") \
                        ("logs."#n".fileformat", "Custom filename format string ( only for type = file") \
                         ("logs."#n".keepopen", "")

#define LOG_INIT(v,n)   if(v.count("logs."#n".enabled") && v.count("logs."#n".type") && v.count("logs."#n".entryformat")) \
                        { \
                            if(v["logs."#n".enabled"].as<std::string>() == "yes") \
                            { \
                                std::string type = v["logs."#n".type"].as<std::string>(); \
                                if("file" == type && v.count("logs."#n".fileformat")) \
                                { \
                                    bool keepopen = false; \
                                    if(v.count("logs."#n".keepopen")) \
                                        keepopen = v["logs."#n".keepopen"].as<std::string>() == "yes"; \
                                    Log::logMap[#n] = new FileLog(new StringFormat(v["logs."#n".fileformat"].as<std::string>()), \
                                                                       new StringFormat(v["logs."#n".entryformat"].as<std::string>()), \
                                                                       keepopen); \
                                } \
                                else if("console" == type) \
                                { \
                                    Log::logMap[#n] = new ConsoleLog(new StringFormat(v["logs."#n".entryformat"].as<std::string>())); \
                                } \
                            } \
                        } \
                        if(!(Log::logMap.count(#n)))\
                        { \
                            Log::logMap[#n] = new VoidLog(); \
                        }

#define LOG(n) (*(Log::logMap[#n]))

#define LOG_ADD(n,l) Log::logMap[#n] = (l);

#endif
