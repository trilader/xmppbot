#include "FileLog.h"

FileLog::FileLog(StringFormat *fileformat, StringFormat *entryformat, bool keepopen)
{
    this->_entryformat = entryformat;
    this->_fileformat = fileformat;

    this->_keepopen = keepopen;

    this->_today = new boost::gregorian::date(boost::gregorian::min_date_time);

    this->updateStream();
}

void FileLog::log(const std::string& msg)
{
    this->updateStream();

    if(!(this->_stream->is_open()))
        this->_stream->open(this->_filename, std::ofstream::out | std::ofstream::app);

    boost::posix_time::ptime t = boost::posix_time::second_clock::local_time();
    this->_entryformat->assign("h",t.time_of_day().hours());
    this->_entryformat->assign("m",t.time_of_day().minutes());
    this->_entryformat->assign("s",t.time_of_day().seconds());

    this->_entryformat->assign("_", msg);

    (*this->_stream) << this->_entryformat->produce() << std::endl;

    this->_stream->flush();

    if(!this->_keepopen)
        this->_stream->close();
}

void FileLog::updateStream()
{
    boost::gregorian::date today(boost::gregorian::day_clock::local_day());

    if((0 != this->_stream) && (today == (*this->_today)))
        return;

    this->_fileformat->assign("d", today.day());
    this->_fileformat->assign("m", today.month());
    this->_fileformat->assign("y", today.year());

    this->_filename = this->_fileformat->produce();

    *this->_today = today;

    if(0 == this->_stream)
        this->_stream = new std::ofstream();
    else if(this->_stream->is_open())
        this->_stream->close();
}
