#include "ConsoleLog.h"

ConsoleLog::ConsoleLog(StringFormat *format)
{
    this->_format = format;
}

void ConsoleLog::log(const std::string& msg)
{
    boost::posix_time::ptime t = boost::posix_time::second_clock::local_time();
    this->_format->assign("h",t.time_of_day().hours());
    this->_format->assign("m",t.time_of_day().minutes());
    this->_format->assign("s",t.time_of_day().seconds());

    this->_format->assign("_", msg);

    std::cout << this->_format->produce() << std::endl;
}
