#ifndef FILELOG_H
#define FILELOG_H

#include "Log.h"
#include "StringFormat.h"
#include <iostream>
#include <fstream>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"

class FileLog : public Log
{
public:
    FileLog(StringFormat *fileformat, StringFormat *entryformat, bool keepopen = false);
    void log(const std::string& msg);

private:
    void updateStream();

    std::ofstream *_stream;
    std::string _filename;

    StringFormat *_fileformat;
    StringFormat *_entryformat;

    bool _keepopen;

    boost::gregorian::date *_today;
};

#endif
