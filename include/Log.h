#ifndef LOG_H
#define LOG_H

#include <string>
#include "boost/lexical_cast.hpp"
#include "boost/unordered_map.hpp"

class Log
{
public:
    virtual void log(const std::string& entry) = 0;
    Log& operator<<(const std::string& msg);

    static boost::unordered_map<std::string, Log* > logMap;

    template <class TIn> void log(const TIn& entry)
    {
        this->log(boost::lexical_cast<std::string>(entry));
    }

    template <class TIn> Log& operator<<(const TIn& msg)
    {
        this->log(msg);
        return *this;
    }
};

#endif
