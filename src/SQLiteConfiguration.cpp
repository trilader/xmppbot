#include "SQLiteConfiguration.h"

SQLiteConfiguration::SQLiteConfiguration(std::string filename, unsigned int index, bool writeable)
{
    this->_writeable = writeable;

    this->_tableFormat = new StringFormat("xmppbot__%table_config");

    this->_dbFile = filename;

    this->initSQLStatements();
    this->setConfigurationIndex(index);

    this->openDatabase();
    this->initTables();
}

bool SQLiteConfiguration::isWritable()
{
    return this->_writeable;
}

void SQLiteConfiguration::initSQLStatements()
{
    this->_selectLogFormat = new StringFormat(std::string("SELECT log_type, log_fileformat, log_entryformat, log_enabled, log_keepopen ")
                                                + "FROM %table WHERE log_name = '%name' AND log_config_id=%index LIMIT 1");
    this->_selectCommandItemFormat = new StringFormat("SELECT command_value FROM %table WHERE command_name = '%name' AND command_option = '%option' AND command_config_id=%index LIMIT 1");
    this->_selectCustomItemFormat = new StringFormat("SELECT custom_value FROM %table WHERE custom_name = '%option' AND custom_config_id=%index LIMIT 1");
    this->_selectFilterItemFormat = new StringFormat("SELECT filter_value FROM %table WHERE filter_name = '%name' AND filter_option = '%option' AND filter_config_id=%index LIMIT 1");
    this->_selectXmppMUCFormat = new StringFormat("SELECT muc_server, muc_room, muc_name FROM %table WHERE muc_config_id=%index LIMIT 1");
    this->_selectXmppUserFormat = new StringFormat("SELECT user_user, user_password, user_address, user_resource FROM %table WHERE user_config_id=%index LIMIT 1");

    this->_deleteCommandItemFormat = new StringFormat("DELETE FROM %table WHERE command_config_id=%index AND command_name='%name' AND command_option='%option'");
    this->_deleteCustomItemFormat = new StringFormat("DELETE FROM %table WHERE custom_config_id=%index AND custom_name='%option'");
    this->_deleteFilterItemFormat = new StringFormat("DELETE FROM %table WHERE filter_config_id=%index AND filter_name='%name' AND filter_option='%option'");
    this->_deleteLogFormat = new StringFormat("DELETE FROM %table WHERE log_config_id=%index");
    this->_deleteXmppMUCFormat = new StringFormat("DELETE FROM %table WHERE muc_config_id=%index");
    this->_deleteXmppUserFormat = new StringFormat("DELETE FROM %table WHERE user_config_id=%index");

    this->_replaceCommandItemFormat = new StringFormat("REPLACE INTO %table (command_config_id, command_name, command_option, command_value) VALUES (%index, '%name', '%option', '%value')");
    this->_replaceCustomItemFormat = new StringFormat("REPLACE INTO %table (custom_config_id, custom_name, custom_value) VALUES (%index, '%option', '%value')");
    this->_replaceFilterItemFormat = new StringFormat("REPLACE INTO %table (filter_config_id, filter_name, filter_option, filter_value) VALUES (%index, '%name', '%option', '%value')");
    this->_replaceXmppMUCFormat = new StringFormat("REPLACE INTO %table (muc_config_id, muc_server, muc_room, muc_name) VALUES (%index, '%server', '%room', '%name')");
    this->_replaceXmppUserFormat = new StringFormat("REPLACE INTO %table (user_config_id, user_user, user_password, user_address, user_resource) VALUES (%index, '%user', '%password', '%address', '%resource')");
    this->_replaceLogFormat = new StringFormat(std::string("REPLACE INTO %table (log_config_id, log_name, log_type, log_fileformat, log_entryformat, log_enabled, log_keepopen) VALUES ")
                                               +"(%index, '%name', '%type', '%fileformat', '%entryformat', %enabled, %keepopen)");

    this->_tableFormat->assign("table", "log");
    this->_tableFormat->produce();
    this->_selectLogFormat->assign("table", this->_tableFormat->last());
    this->_deleteLogFormat->assign("table", this->_tableFormat->last());
    this->_replaceLogFormat->assign("table", this->_tableFormat->last());

    this->_tableFormat->assign("table", "command");
    this->_tableFormat->produce();
    this->_selectCommandItemFormat->assign("table", this->_tableFormat->last());
    this->_deleteCommandItemFormat->assign("table", this->_tableFormat->last());
    this->_replaceCommandItemFormat->assign("table", this->_tableFormat->last());

    this->_tableFormat->assign("table", "custom");
    this->_tableFormat->produce();
    this->_selectCustomItemFormat->assign("table", this->_tableFormat->last());
    this->_deleteCustomItemFormat->assign("table", this->_tableFormat->last());
    this->_replaceCustomItemFormat->assign("table", this->_tableFormat->last());

    this->_tableFormat->assign("table", "filter");
    this->_tableFormat->produce();
    this->_selectFilterItemFormat->assign("table", this->_tableFormat->last());
    this->_deleteFilterItemFormat->assign("table", this->_tableFormat->last());
    this->_replaceFilterItemFormat->assign("table", this->_tableFormat->last());

    this->_tableFormat->assign("table", "muc");
    this->_tableFormat->produce();
    this->_selectXmppMUCFormat->assign("table", this->_tableFormat->last());
    this->_deleteXmppMUCFormat->assign("table", this->_tableFormat->last());
    this->_replaceXmppMUCFormat->assign("table", this->_tableFormat->last());

    this->_tableFormat->assign("table", "user");
    this->_tableFormat->produce();
    this->_selectXmppUserFormat->assign("table", this->_tableFormat->last());
    this->_deleteXmppUserFormat->assign("table", this->_tableFormat->last());
    this->_replaceXmppUserFormat->assign("table", this->_tableFormat->last());
}

unsigned int SQLiteConfiguration::getConfigurationIndex()
{
    return this->_index;
}

bool SQLiteConfiguration::setConfigurationIndex(unsigned int index)
{
    this->_index = index;

    this->_selectLogFormat->assign("index", index);
    this->_selectCommandItemFormat->assign("index", index);
    this->_selectCustomItemFormat->assign("index", index);
    this->_selectFilterItemFormat->assign("index", index);
    this->_selectXmppMUCFormat->assign("index", index);
    this->_selectXmppUserFormat->assign("index", index);

    this->_deleteLogFormat->assign("index", index);
    this->_deleteCommandItemFormat->assign("index", index);
    this->_deleteCustomItemFormat->assign("index", index);
    this->_deleteFilterItemFormat->assign("index", index);
    this->_deleteXmppMUCFormat->assign("index", index);
    this->_deleteXmppUserFormat->assign("index", index);

    this->_replaceLogFormat->assign("index", index);
    this->_replaceCommandItemFormat->assign("index", index);
    this->_replaceCustomItemFormat->assign("index", index);
    this->_replaceFilterItemFormat->assign("index", index);
    this->_replaceXmppMUCFormat->assign("index", index);
    this->_replaceXmppUserFormat->assign("index", index);

    return true;
}

void SQLiteConfiguration::openDatabase()
{
    if(0 != this->_db && this->_db->Connected())
        return;

    if(0 != this->_db)
        delete this->_db;

    this->_db = new Database(this->_dbFile);
}

void SQLiteConfiguration::initTables()
{
    //TODO
}

bool SQLiteConfiguration::setLog(const std::string& name, const std::string& type, bool enabled, bool keepopen)
{
    return this->setLog(name,type,"","",enabled,keepopen);
}

bool SQLiteConfiguration::setLog(const std::string& name, const std::string& type, const std::string& fileformat, const std::string& entryformat, bool enabled, bool keepopen)
{
    if(!this->isWritable())
        return false;

    if("" == name)
        return false;

    this->openDatabase();

    LoggableQuery query(*this->_db);

    if("" == type)
        return query.executeAndLog(this->_deleteLogFormat->produce());

    this->_replaceLogFormat->assign("type", type);
    this->_replaceLogFormat->assign("fileformat", fileformat);
    this->_replaceLogFormat->assign("entryformat", entryformat);
    this->_replaceLogFormat->assign("enabled", (enabled ? "1" : "0"));
    this->_replaceLogFormat->assign("keepopen", (keepopen ? "1" : "0"));

    return query.executeAndLog(this->_replaceLogFormat->produce());
}

bool SQLiteConfiguration::setXmppUser(const std::string& user, const std::string& password, const std::string& address, const std::string& resource)
{
    if(!this->isWritable())
        return false;

    this->openDatabase();

    LoggableQuery query(*this->_db);

    if("" == user)
        return query.executeAndLog(this->_deleteXmppUserFormat->produce());

    this->_replaceXmppUserFormat->assign("user", user);
    this->_replaceXmppUserFormat->assign("password", password);
    this->_replaceXmppUserFormat->assign("address", password);
    this->_replaceXmppUserFormat->assign("resource", resource);

    return query.executeAndLog(this->_replaceXmppUserFormat->produce());
}

bool SQLiteConfiguration::setXmppMUC(const std::string& name, const std::string& room, const std::string& server)
{
    if(!this->isWritable())
        return false;

    this->openDatabase();

    LoggableQuery query(*this->_db);

    if("" == name)
        return query.executeAndLog(this->_deleteXmppMUCFormat->produce());

    this->_replaceXmppMUCFormat->assign("name", name);
    this->_replaceXmppMUCFormat->assign("room", room);
    this->_replaceXmppMUCFormat->assign("server", server);

    return query.executeAndLog(this->_replaceXmppMUCFormat->produce());
}

bool SQLiteConfiguration::setOptionByFormat(StringFormat *format, const std::string& name, const std::string& option, const std::string& value)
{
    format->assign("name", name);
    format->assign("option", option);
    format->assign("value", value);

    this->openDatabase();

    LoggableQuery query(*this->_db);

    return query.executeAndLog(format->produce());
}

bool SQLiteConfiguration::setCustomCommandItem(const std::string& cmd, const std::string& name, const std::string& value)
{
    if("" == cmd || "" == name || !this->isWritable())
        return false;

    if("" == value)
        return this->setOptionByFormat(this->_deleteCommandItemFormat, cmd, name, value);

    return this->setOptionByFormat(this->_replaceCommandItemFormat, cmd, name, value);
}

bool SQLiteConfiguration::setCustomFilterItem(const std::string& filter, const std::string& name, const std::string& value)
{
    if("" == filter || "" == name || !this->isWritable())
        return false;

    if("" == value)
        return this->setOptionByFormat(this->_deleteFilterItemFormat, filter, name, value);

    return this->setOptionByFormat(this->_replaceFilterItemFormat, filter, name, value);
}

bool SQLiteConfiguration::setCustomItem(const std::string& name, const std::string& value)
{
    if("" == name || !this->isWritable())
        return false;

    if("" == value)
        return this->setOptionByFormat(this->_deleteCustomItemFormat, "", name, value);

    return this->setOptionByFormat(this->_replaceCustomItemFormat, "", name, value);
}

bool SQLiteConfiguration::getLog(const std::string& name, std::string *type, bool *enabled, bool *keepopen)
{
    std::string dummy0, dummy1;
    return this->getLog(name, type, &dummy0, &dummy1, enabled, keepopen);
}

bool SQLiteConfiguration::getLog(const std::string& name, std::string *type, std::string *fileformat, std::string *entryformat, bool *enabled, bool *keepopen)
{
    this->openDatabase();

    LoggableQuery query(*this->_db);

    this->_selectLogFormat->assign("name", name);

    *type = ""; *fileformat = ""; *entryformat = ""; *enabled = false; *keepopen = false;

    query.get_result(this->_selectLogFormat->produce());
    if(query.num_rows() <= 0)
    {
        query.free_result();
        return false;
    }

    query.fetch_row();

    *type = query.getstr("log_type");
    *fileformat = query.getstr("log_fileformat");
    *entryformat = query.getstr("log_entryformat");
    *enabled = query.getnum("log_enabled") > 0;
    *keepopen = query.getnum("log_keepopen") > 0;

    query.free_result();
    return true;
}

bool SQLiteConfiguration::getXmppUser(std::string *user, std::string *password, std::string *address, std::string *resource)
{
    this->openDatabase();

    *user = ""; *password = ""; *address = ""; *resource = "";

    LoggableQuery query(*this->_db);

    query.get_result(this->_selectXmppUserFormat->produce());
    if(query.num_rows() <= 0)
    {
        query.free_result();
        return false;
    }

    query.fetch_row();

    *user = query.getstr("user_user");
    *password = query.getstr("user_password");
    *address = query.getstr("user_address");
    *resource = query.getstr("user_resource");

    query.free_result();

    return true;
}

bool SQLiteConfiguration::getXmppMUC(std::string *name, std::string *room, std::string *server)
{
    this->openDatabase();

    *name = ""; *room = ""; *server = "";

    LoggableQuery query(*this->_db);

    query.get_result(this->_selectXmppMUCFormat->produce());
    if(query.num_rows() <= 0)
    {
        query.free_result();
        return false;
    }

    query.fetch_row();

    *name = query.getstr("muc_name");
    *room = query.getstr("muc_room");
    *server = query.getstr("muc_server");

    query.free_result();

    return true;
}

bool SQLiteConfiguration::getOptionByFormat(StringFormat *format, const std::string& name, const std::string& option, std::string *value)
{
    this->openDatabase();

    format->assign("name", name);
    format->assign("option", option);

    LoggableQuery query(*this->_db);

    *value = "";

    query.get_result(format->produce());
    if(query.num_rows() <= 0)
    {
        query.free_result();

        return false;
    }

    query.fetch_row();

    *value = query.getstr();

    query.free_result();

    return true;
}

std::string SQLiteConfiguration::getCustomCommandItem(const std::string& cmd, const std::string& option)
{
    std::string value;
    this->getOptionByFormat(this->_selectCommandItemFormat, cmd, option, &value);

    return value;
}

std::string SQLiteConfiguration::getCustomFilterItem(const std::string& filter, const std::string& option)
{
    std::string value;
    this->getOptionByFormat(this->_selectFilterItemFormat, filter, option, &value);

    return value;
}

std::string SQLiteConfiguration::getCustomItem(const std::string& name)
{
    std::string value;
    this->getOptionByFormat(this->_selectFilterItemFormat, "", name, &value);

    return value;
}

bool SQLiteConfiguration::isCustomCommandItemSet(const std::string& cmd, const std::string& option)
{
    std::string value;
    return this->getOptionByFormat(this->_selectCommandItemFormat, cmd, option, &value);
}

bool SQLiteConfiguration::isCustomFilterItemSet(const std::string& filter, const std::string& option)
{
    std::string value;
    return this->getOptionByFormat(this->_selectFilterItemFormat, filter, option, &value);
}

bool SQLiteConfiguration::isCustomItemSet(const std::string& name)
{
    std::string value;
    return this->getOptionByFormat(this->_selectFilterItemFormat, "", name, &value);
}
