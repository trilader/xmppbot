#ifdef DB_SUPPORT

#include "DbConfiguration.h"

DbConfiguration::DbConfiguration(const soci::backend_factory& factory, std::string connString, bool writeable, unsigned int index) : _factory(factory)
{
    this->_db = 0;
    this->_writeable = writeable;

    this->_tableFormat = new StringFormat("xmppbot__%table_config");

    this->_connString = connString;

    this->initSQLStatements();
    this->setConfigurationIndex(index);

    this->openDatabase();
    this->initTables();
}

bool DbConfiguration::isWritable()
{
    return this->_writeable;
}

void DbConfiguration::initSQLStatements()
{
    this->_selectLogFormat = new StringFormat(std::string("SELECT logs_type, logs_fileformat, logs_entryformat, logs_enabled, logs_keepopen ")
                                                + "FROM %table WHERE logs_name = :name AND logs_config_id=%index LIMIT 1");
    this->_selectCommandItemFormat = new StringFormat("SELECT command_value FROM %table WHERE command_name = :name AND command_option = :option AND command_config_id=%index LIMIT 1");
    this->_selectCustomItemFormat = new StringFormat("SELECT custom_value FROM %table WHERE custom_name = '%option' AND custom_config_id=%index LIMIT 1");
    this->_selectFilterItemFormat = new StringFormat("SELECT filter_value FROM %table WHERE filter_name = :name AND filter_option = :option AND filter_config_id=%index LIMIT 1");
    this->_selectXmppMUCFormat = new StringFormat("SELECT muc_server, muc_room, muc_name FROM %table WHERE muc_config_id=%index LIMIT 1");
    this->_selectXmppUserFormat = new StringFormat("SELECT server_user, server_password, server_address, server_resource FROM %table WHERE server_config_id=%index LIMIT 1");

    this->_deleteCommandItemFormat = new StringFormat("DELETE FROM %table WHERE command_config_id=%index AND command_name=:name AND command_option=:option");
    this->_deleteCustomItemFormat = new StringFormat("DELETE FROM %table WHERE custom_config_id=%index AND custom_name=:option");
    this->_deleteFilterItemFormat = new StringFormat("DELETE FROM %table WHERE filter_config_id=%index AND filter_name=:name AND filter_option=:option");
    this->_deleteLogFormat = new StringFormat("DELETE FROM %table WHERE logs_config_id=%index AND logs_name=:name");
    this->_deleteXmppMUCFormat = new StringFormat("DELETE FROM %table WHERE muc_config_id=%index");
    this->_deleteXmppUserFormat = new StringFormat("DELETE FROM %table WHERE server_config_id=%index");

    this->_replaceCommandItemFormat = new StringFormat("REPLACE INTO %table (command_config_id, command_name, command_option, command_value) VALUES (%index, :name, :option, :value)");
    this->_replaceCustomItemFormat = new StringFormat("REPLACE INTO %table (custom_config_id, custom_name, custom_value) VALUES (%index, :option, :value)");
    this->_replaceFilterItemFormat = new StringFormat("REPLACE INTO %table (filter_config_id, filter_name, filter_option, filter_value) VALUES (%index, :name, :option,:value)");
    this->_replaceXmppMUCFormat = new StringFormat("REPLACE INTO %table (muc_config_id, muc_server, muc_room, muc_name) VALUES (%index, :server, :room, :name)");
    this->_replaceXmppUserFormat = new StringFormat("REPLACE INTO %table (server_config_id, server_user, server_password, server_address, server_resource) VALUES (%index, :user, :password, :address, :resource)");
    this->_replaceLogFormat = new StringFormat(std::string("REPLACE INTO %table (logs_config_id, logs_name, logs_type, logs_fileformat, logs_entryformat, logs_enabled, logs_keepopen) VALUES ")
                                               +"(%index, :name, :type, :fileformat, :entryformat, :enabled, :keepopen)");

    this->_tableFormat->assign("table", this->getLogOptionIdent());
    this->_tableFormat->produce();
    this->_selectLogFormat->assign("table", this->_tableFormat->last());
    this->_deleteLogFormat->assign("table", this->_tableFormat->last());
    this->_replaceLogFormat->assign("table", this->_tableFormat->last());

    this->_tableFormat->assign("table", this->getCommandOptionIdent());
    this->_tableFormat->produce();
    this->_selectCommandItemFormat->assign("table", this->_tableFormat->last());
    this->_deleteCommandItemFormat->assign("table", this->_tableFormat->last());
    this->_replaceCommandItemFormat->assign("table", this->_tableFormat->last());

    this->_tableFormat->assign("table", this->getCustomOptionIdent());
    this->_tableFormat->produce();
    this->_selectCustomItemFormat->assign("table", this->_tableFormat->last());
    this->_deleteCustomItemFormat->assign("table", this->_tableFormat->last());
    this->_replaceCustomItemFormat->assign("table", this->_tableFormat->last());

    this->_tableFormat->assign("table", this->getFilterOptionIdent());
    this->_tableFormat->produce();
    this->_selectFilterItemFormat->assign("table", this->_tableFormat->last());
    this->_deleteFilterItemFormat->assign("table", this->_tableFormat->last());
    this->_replaceFilterItemFormat->assign("table", this->_tableFormat->last());

    this->_tableFormat->assign("table", this->getMUCOptionIdent());
    this->_tableFormat->produce();
    this->_selectXmppMUCFormat->assign("table", this->_tableFormat->last());
    this->_deleteXmppMUCFormat->assign("table", this->_tableFormat->last());
    this->_replaceXmppMUCFormat->assign("table", this->_tableFormat->last());

    this->_tableFormat->assign("table", this->getUserOptionIdent());
    this->_tableFormat->produce();
    this->_selectXmppUserFormat->assign("table", this->_tableFormat->last());
    this->_deleteXmppUserFormat->assign("table", this->_tableFormat->last());
    this->_replaceXmppUserFormat->assign("table", this->_tableFormat->last());
}

unsigned int DbConfiguration::getConfigurationIndex()
{
    return this->_index;
}

bool DbConfiguration::setConfigurationIndex(unsigned int index)
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

void DbConfiguration::openDatabase()
{
    if(0 != this->_db)
        return;

    this->_db = new soci::session(this->_factory, this->_connString);
}

void DbConfiguration::initTables()
{
}

bool DbConfiguration::setLog(const std::string& name, const std::string& type, bool enabled, bool keepopen)
{
    return this->setLog(name,type,"","",enabled,keepopen);
}

bool DbConfiguration::setLog(const std::string& name, const std::string& type, const std::string& fileformat, const std::string& entryformat, bool enabled, bool keepopen)
{
    if(!this->isWritable())
        return false;

    if("" == name)
        return false;

    this->openDatabase();


    if("" == type && "" == fileformat && "" == entryformat && !enabled && !keepopen)
    {
        *this->_db << this->_deleteLogFormat->produce(), soci::use(name);

        return true;
    }

    *this->_db << this->_replaceLogFormat->produce(),
                    soci::use(name,"name"),
                    soci::use(type,"type"),
                    soci::use(fileformat,"fileformat"),
                    soci::use(entryformat, "entryformat"),
                    soci::use(enabled ? 1 : 0, "enabled"),
                    soci::use(keepopen ? 1 : 0, "keepopen");

    return true;
}

bool DbConfiguration::setXmppUser(const std::string& user, const std::string& password, const std::string& address, const std::string& resource)
{
    if(!this->isWritable())
        return false;

    this->openDatabase();

    if("" == user && "" == password && "" == address && "" == resource)
    {
        *this->_db << this->_deleteXmppUserFormat->produce();
        return true;
    }

    *this->_db << this->_replaceXmppUserFormat->produce(),
                    soci::use(user,"user"),
                    soci::use(password,"password"),
                    soci::use(address,"address"),
                    soci::use(resource,"resource");

    return true;
}

bool DbConfiguration::setXmppMUC(const std::string& name, const std::string& room, const std::string& server)
{
    if(!this->isWritable())
        return false;

    this->openDatabase();

    if("" == name && "" == room && "" == server)
    {
        *this->_db << this->_deleteXmppMUCFormat->produce();
        return true;
    }

    *this->_db << this->_replaceXmppMUCFormat->produce(),
                    soci::use(name, "name"),
                    soci::use(room, "room"),
                    soci::use(server, "server");

    return true;
}

bool DbConfiguration::setOptionByFormat(StringFormat *format, const std::string& name, const std::string& option, const std::string& value)
{
    format->assign("name", name);
    format->assign("option", option);
    format->assign("value", value);

    this->openDatabase();

    *this->_db << format->produce(),
        soci::use(name, "name"),
        soci::use(option, "option"),
        soci::use(value, "value");

    return true;
}

bool DbConfiguration::setCustomCommandItem(const std::string& cmd, const std::string& name, const std::string& value)
{
    if("" == cmd || "" == name || !this->isWritable())
        return false;

    if("" == value)
        return this->setOptionByFormat(this->_deleteCommandItemFormat, cmd, name, value);

    return this->setOptionByFormat(this->_replaceCommandItemFormat, cmd, name, value);
}

bool DbConfiguration::setCustomFilterItem(const std::string& filter, const std::string& name, const std::string& value)
{
    if("" == filter || "" == name || !this->isWritable())
        return false;

    if("" == value)
        return this->setOptionByFormat(this->_deleteFilterItemFormat, filter, name, value);

    return this->setOptionByFormat(this->_replaceFilterItemFormat, filter, name, value);
}

bool DbConfiguration::setCustomItem(const std::string& name, const std::string& value)
{
    if("" == name || !this->isWritable())
        return false;

    if("" == value)
        return this->setOptionByFormat(this->_deleteCustomItemFormat, "", name, value);

    return this->setOptionByFormat(this->_replaceCustomItemFormat, "", name, value);
}

bool DbConfiguration::getLog(const std::string& name, std::string *type, bool *enabled, bool *keepopen)
{
    std::string dummy0, dummy1;
    return this->getLog(name, type, &dummy0, &dummy1, enabled, keepopen);
}

bool DbConfiguration::getLog(const std::string& name, std::string *type, std::string *fileformat, std::string *entryformat, bool *enabled, bool *keepopen)
{
    this->openDatabase();

    this->_selectLogFormat->assign("name", name);

    *type = ""; *fileformat = ""; *entryformat = ""; *enabled = false; *keepopen = false;
    int enabled_buff, keepopen_buff;

    *this->_db << this->_selectLogFormat->produce(),
                    soci::use(name, "name"),
                    soci::into(*type),
                    soci::into(*fileformat),
                    soci::into(*entryformat),
                    soci::into(enabled_buff),
                    soci::into(keepopen_buff);

    *enabled = enabled_buff > 0;
    *keepopen = keepopen_buff > 0;

    return true;
}

bool DbConfiguration::getXmppUser(std::string *user, std::string *password, std::string *address, std::string *resource)
{
    this->openDatabase();

    *user = ""; *password = ""; *address = ""; *resource = "";

    *this->_db << this->_selectXmppUserFormat->produce(),
                    soci::into(*user),
                    soci::into(*password),
                    soci::into(*address),
                    soci::into(*resource);

    return true;
}

bool DbConfiguration::getXmppMUC(std::string *name, std::string *room, std::string *server)
{
    this->openDatabase();

    *name = ""; *room = ""; *server = "";

    *this->_db << this->_selectXmppMUCFormat->produce(),
                    soci::into(*server),
                    soci::into(*room),
                    soci::into(*name);

    return true;
}

bool DbConfiguration::getOptionByFormat(StringFormat *format, const std::string& name, const std::string& option, std::string *value)
{
    this->openDatabase();

    format->assign("name", name);
    format->assign("option", option);

    *this->_db << format->produce(),
        soci::use(name, "name"),
        soci::use(option, "option"),
        soci::into(*value);

    return true;
}

std::string DbConfiguration::getCustomCommandItem(const std::string& cmd, const std::string& option)
{
    std::string value;
    this->getOptionByFormat(this->_selectCommandItemFormat, cmd, option, &value);

    return value;
}

std::string DbConfiguration::getCustomFilterItem(const std::string& filter, const std::string& option)
{
    std::string value;
    this->getOptionByFormat(this->_selectFilterItemFormat, filter, option, &value);

    return value;
}

std::string DbConfiguration::getCustomItem(const std::string& name)
{
    std::string value;
    this->getOptionByFormat(this->_selectFilterItemFormat, "", name, &value);

    return value;
}

bool DbConfiguration::isCustomCommandItemSet(const std::string& cmd, const std::string& option)
{
    std::string value;
    return this->getOptionByFormat(this->_selectCommandItemFormat, cmd, option, &value);
}

bool DbConfiguration::isCustomFilterItemSet(const std::string& filter, const std::string& option)
{
    std::string value;
    return this->getOptionByFormat(this->_selectFilterItemFormat, filter, option, &value);
}

bool DbConfiguration::isCustomItemSet(const std::string& name)
{
    std::string value;
    return this->getOptionByFormat(this->_selectFilterItemFormat, "", name, &value);
}

#endif
