#include "ConsoleLog.h"

ConsoleLog::ConsoleLog(StringFormat *format)
{
    this->_format = format;
}

void ConsoleLog::log(const std::string& msg)
{
    assignFormatDateTime(_format);

    this->_format->assign("_", msg);

    std::cout << this->_format->produce() << std::endl;
}
