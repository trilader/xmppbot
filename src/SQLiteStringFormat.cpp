#include "SQLiteStringFormat.h"

SQLiteStringFormat::SQLiteStringFormat(std::string format, std::string varprefix) : StringFormat(format, varprefix)
{
    this->_clearMap = new boost::unordered_set<std::string>();
}

//TODO: assign
void SQLiteStringFormat::assign(const std::string& name,const std::string& value)
{
    this->_clearMap->erase(name);

    StringFormat::assign(name, value);
}

void SQLiteStringFormat::safeVariables(Database *db)
{
    for (boost::unordered_map<std::string, std::string>::iterator it = this->_assignments->begin(); it != this->_assignments->end(); ++it)
        if(this->_clearMap->insert(it->second).second)
            it->second = db->xmlsafestr(db->safestr(it->second));
}

std::string SQLiteStringFormat::produce(Database *db)
{
    this->safeVariables(db);

    return StringFormat::produce();
}
