#include "Log.h"

Log& Log::operator<<(const std::string& msg)
{
    this->log(msg);

    return *this;
}

boost::unordered_map<std::string, Log* > Log::logMap;
