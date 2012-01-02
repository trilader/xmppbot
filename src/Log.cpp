#include "Log.h"

Log& Log::operator<<(const std::string& msg)
{
    this->log(msg);

    return *this;
}
void Log::assignFormatDateTime(StringFormat* _format)
{
    boost::gregorian::date today = boost::gregorian::day_clock::local_day();
    boost::posix_time::ptime t = boost::posix_time::second_clock::local_time();

    _format->assign("Y",today.year());
    _format->assign("M",today.month());
    _format->assign("D",today.day());

    _format->assign("h",t.time_of_day().hours());
    _format->assign("m",t.time_of_day().minutes());
    _format->assign("s",t.time_of_day().seconds());
}
