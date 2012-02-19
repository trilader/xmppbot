#ifndef SQLITESTRINGFORMAT_H
#define SQLITESTRINGFORMAT_H

#include "StringFormat.h"
#include "DatabaseHelper.h"
#include "boost/unordered_set.hpp"

class SQLiteStringFormat : public StringFormat
{
    public:
        SQLiteStringFormat(std::string format, std::string varprefix = "%");

        void assign(const std::string& name,const std::string& value);

        template <class TIn> void assign(const std::string& name, const TIn& value)
        {
            StringFormat::assign(name, value);
        }

    private:
        boost::unordered_set<std::string> *_clearMap;
};

#endif
