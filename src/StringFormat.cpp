#include "StringFormat.h"

StringFormat::StringFormat(std::string format, std::string varprefix)
{
    this->_format = format;
    this->_prefix = varprefix;
    this->_last = "";
    this->_assignments = new boost::unordered_map<std::string, std::string>();
}

void StringFormat::assign(const std::string& name, const std::string& value)
{
    (*(this->_assignments))[name] = value.substr(0);
}

std::string StringFormat::last()
{
    return this->_last;
}

std::string StringFormat::produce()
{
    std::string product = this->_format.substr(0);

    for (boost::unordered_map<std::string, std::string>::iterator it = this->_assignments->begin(); it != this->_assignments->end(); ++it)
        boost::algorithm::replace_all(product, this->_prefix + it->first, it->second);

    this->_last = product;

    return this->_last;
}

std::string StringFormat::getFormatString()
{
    return this->_format.substr(0);
}
