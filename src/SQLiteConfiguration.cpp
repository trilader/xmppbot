#include "SQLiteConfiguration.h"

SQLiteConfiguration::SQLiteConfiguration(std::string filename, bool writeable, unsigned int index) : DbConfiguration(soci::sqlite3,filename, writeable, index)
{
}

void SQLiteConfiguration::initTables()
{
    StringFormat createLogTable(std::string("CREATE TABLE IF NOT EXISTS %table (")
                                                   +" logs_config_id integer not null,"
                                                   +" logs_name varchar(255) not null, "
                                                   +" logs_type varchar(255) default '' not null,"
                                                   +" logs_fileformat varchar(255) default '' not null,"
                                                   +" logs_entryformat varchar(255) default '' not null,"
                                                   +" logs_enabled integer default 0 not null,"
                                                   +" logs_keepopen integer default 0 not null,"
                                                   +" PRIMARY KEY (logs_config_id, logs_name));");

    StringFormat createMUCTable(std::string("CREATE TABLE IF NOT EXISTS %table (")
                                                   +" muc_config_id integer primary key not null,"
                                                   +" muc_name varchar(255) default '' not null, "
                                                   +" muc_room varchar(255) default '' not null,"
                                                   +" muc_server varchar(255) default '' not null);");

    StringFormat createUserTable(std::string("CREATE TABLE IF NOT EXISTS %table (")
                                                    +" server_config_id integer primary key not null,"
                                                    +" server_user varchar(255) default '' not null,"
                                                    +" server_password varchar(255) default '' not null,"
                                                    +" server_address varchar(255) default '' not null,"
                                                    +" server_resource varchar(255) default '' not null);");

    StringFormat createCommandTable(std::string("CREATE TABLE IF NOT EXISTS %table (")
                                                       +" command_config_id integer not null,"
                                                       +" command_name varchar(255) default '' not null,"
                                                       +" command_option varchar(255) default '' not null,"
                                                       +" command_value varchar(255) default '' not null,"
                                                       +" PRIMARY KEY (command_config_id, command_name, command_option));");

    StringFormat createFilterTable(std::string("CREATE TABLE IF NOT EXISTS %table (")
                                                       +" filter_config_id integer not null,"
                                                       +" filter_name varchar(255) default '' not null,"
                                                       +" filter_option varchar(255) default '' not null,"
                                                       +" filter_value varchar(255) default '' not null,"
                                                       +" PRIMARY KEY (filter_config_id, filter_name, filter_option));");


    StringFormat createCustomTable(std::string("CREATE TABLE IF NOT EXISTS %table (")
                                                       +" custom_config_id integer not null,"
                                                       +" custom_name varchar(255) default '' not null,"
                                                       +" custom_value varchar(255) default '' not null,"
                                                       +" PRIMARY KEY (custom_config_id, custom_name));");

    this->_tableFormat->assign("table", this->getLogOptionIdent());
    createLogTable.assign("table", this->_tableFormat->produce());

    this->_tableFormat->assign("table", this->getUserOptionIdent());
    createUserTable.assign("table", this->_tableFormat->produce());

    this->_tableFormat->assign("table", this->getMUCOptionIdent());
    createMUCTable.assign("table", this->_tableFormat->produce());

    this->_tableFormat->assign("table", this->getCommandOptionIdent());
    createCommandTable.assign("table", this->_tableFormat->produce());

    this->_tableFormat->assign("table", this->getFilterOptionIdent());
    createFilterTable.assign("table", this->_tableFormat->produce());

    this->_tableFormat->assign("table", this->getCustomOptionIdent());
    createCustomTable.assign("table", this->_tableFormat->produce());

    *this->_db << createLogTable.produce();
    *this->_db << createCommandTable.produce();
    *this->_db << createCustomTable.produce();
    *this->_db << createFilterTable.produce();
    *this->_db << createMUCTable.produce();
    *this->_db << createUserTable.produce();
}
