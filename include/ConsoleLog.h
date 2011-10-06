#ifndef CONSOLELOG_H
#define CONSOLELOG_H

#include <iostream>
#include "Log.h"
#include "StringFormat.h"
#include "boost/date_time/posix_time/posix_time.hpp"

class ConsoleLog : public Log
{
public:
    ConsoleLog(StringFormat *format);
    void log(const std::string& msg);

private:
    StringFormat *_format;
};

#endif
