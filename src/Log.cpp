#include "Log.h"

Log& Log::operator<<(const std::string& msg)
{
    this->log(msg);

    return *this;
}
void Log::assignFormatDateTime(StringFormat* _format)
{
    boost::gregorian::date today = boost::gregorian::day_clock::local_day();
    boost::gregorian::date::ymd_type ymd = today.year_month_day();
    boost::posix_time::ptime t = boost::posix_time::second_clock::local_time();

    std::stringstream str;
    str.width(2);
    str.fill('0');

    long hours = t.time_of_day().hours();
    long minutes = t.time_of_day().minutes();
    long seconds = t.time_of_day().seconds();
    long month = ymd.month;

    _format->assign("Y",ymd.year);
    
    str<<month; // store the month in a long first to get the number instead of the name.
    _format->assign("M",str.str());
    str.str("");

    str<<ymd.day;
    _format->assign("D",str.str());
    str.str("");

    str<<hours;
    _format->assign("h",str.str());
    str.str("");

    str<<minutes;
    _format->assign("m",str.str());
    str.str("");

    str<<seconds;
    _format->assign("s",str.str());
    str.str("");
}
