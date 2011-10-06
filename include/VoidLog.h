#ifndef VOIDLOG_H
#define VOIDLOG_H

#include "Log.h"

class VoidLog : public Log
{
public:
    void log(const std::string& msg);
};

#endif
