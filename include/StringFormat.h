#ifndef STRINGFORMAT_H
#define STRINGFORMAT_H

#include <string>
#include "boost/algorithm/string/replace.hpp"
#include "boost/unordered_map.hpp"
#include "boost/lexical_cast.hpp"

class StringFormat
{
public:
    StringFormat(std::string format, std::string varprefix = "%");
    void assign(const std::string& name,const std::string& value);

    std::string produce();
    std::string last();

    std::string getFormatString();

    template <class TIn> void assign(const std::string& name, const TIn& value)
    {
        this->assign(name, boost::lexical_cast<std::string>(value));
    }

private:
    std::string _format;
    std::string _prefix;

    std::string _last;

    boost::unordered_map<std::string, std::string> *_assignments;
};

#endif
