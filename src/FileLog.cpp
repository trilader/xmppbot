#include "FileLog.h"

FileLog::FileLog(StringFormat *fileformat, StringFormat *entryformat, bool keepopen)
{
    this->_entryformat = entryformat;
    this->_fileformat = fileformat;

    this->_keepopen = keepopen;

    this->_today = new boost::gregorian::date(boost::gregorian::min_date_time);
    this->_thishour = boost::posix_time::second_clock::local_time().time_of_day().hours();

    this->_stream = 0;
    this->updateStream();
}

void FileLog::log(const std::string& msg)
{
    this->updateStream();

    if(!(this->_stream->is_open()))
        this->_stream->open(this->_filename, std::ofstream::out | std::ofstream::app);

    assignFormatDateTime(_entryformat);

    this->_entryformat->assign("_", msg);

    (*this->_stream) << this->_entryformat->produce() << std::endl;

    this->_stream->flush();

    if(!this->_keepopen)
        this->_stream->close();
}

void FileLog::updateStream()
{
    boost::gregorian::date today = boost::gregorian::day_clock::local_day();
    boost::posix_time::ptime t = boost::posix_time::second_clock::local_time();

    if((0 != this->_stream) && (today == (*this->_today)) && (t.time_of_day().hours() == (this->_thishour)))
        return;
        std::cout<<_entryformat->getFormatString()<<std::endl;
    assignFormatDateTime(_fileformat);

    this->_filename = this->_fileformat->produce();

    *this->_today = today;
    this->_thishour = t.time_of_day().hours();

    if(0 == this->_stream)
         this->_stream = new std::ofstream();
    else if(this->_stream->is_open())
        this->_stream->close();
}
