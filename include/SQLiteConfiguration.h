#ifndef SQLITECONFIGURATION_H
#define SQLITECONFIGURATION_H

#include "Configuration.h"
#include "DatabaseHelper.h"
#include "SQLiteStringFormat.h"
#include "LoggableQuery.h"

class SQLiteConfiguration : public Configuration
{
    public:
        SQLiteConfiguration(std::string filename, unsigned int index = 0, bool writeable = true);

        virtual bool setConfigurationIndex(unsigned int index);
        virtual unsigned int getConfigurationIndex();

        virtual bool isWritable();

        //Logs
        virtual bool setLog(const std::string& name, const std::string& type, bool enabled, bool keepopen);
        virtual bool setLog(const std::string& name, const std::string& type, const std::string& fileformat, const std::string& entryformat, bool enabled, bool keepopen);
        virtual bool getLog(const std::string& name, std::string *type, bool *enabled, bool *keepopen);
        virtual bool getLog(const std::string& name, std::string *type, std::string *fileformat, std::string *entryformat, bool *enabled, bool *keepopen);

        //Custom
        virtual bool setCustomItem(const std::string& name, const std::string& value);
        virtual std::string getCustomItem(const std::string& name);
        virtual bool isCustomItemSet(const std::string& name);

        //Commands
        virtual bool setCustomCommandItem(const std::string& cmd, const std::string& name, const std::string& value);
        virtual std::string getCustomCommandItem(const std::string& cmd, const std::string& name);
        virtual bool isCustomCommandItemSet(const std::string& cmd, const std::string& name);

        //Filter
        virtual bool setCustomFilterItem(const std::string& filter, const std::string& name, const std::string& value);
        virtual std::string getCustomFilterItem(const std::string& filter, const std::string& name);
        virtual bool isCustomFilterItemSet(const std::string& filter, const std::string& name);

        //Xmpp
        virtual bool setXmppUser(const std::string& user, const std::string& password, const std::string& address, const std::string& resource);
        virtual bool getXmppUser(std::string *user, std::string *password, std::string *address, std::string *resource);
        virtual bool setXmppMUC(const std::string& name, const std::string& room, const std::string& server);
        virtual bool getXmppMUC(std::string *name, std::string *room, std::string *server);

    private:
        void initSQLStatements();
        void initTables();
        void openDatabase();
        bool setOptionByFormat(SQLiteStringFormat *format, const std::string& name, const std::string& option, const std::string& value);
        bool getOptionByFormat(SQLiteStringFormat *format, const std::string& name, const std::string& option, std::string *value);

        StringFormat *_tableFormat;

        SQLiteStringFormat *_selectLogFormat;
        SQLiteStringFormat *_selectXmppUserFormat;
        SQLiteStringFormat *_selectXmppMUCFormat;
        SQLiteStringFormat *_selectCommandItemFormat;
        SQLiteStringFormat *_selectFilterItemFormat;
        SQLiteStringFormat *_selectCustomItemFormat;

        SQLiteStringFormat *_deleteLogFormat;
        SQLiteStringFormat *_deleteXmppUserFormat;
        SQLiteStringFormat *_deleteXmppMUCFormat;
        SQLiteStringFormat *_deleteCommandItemFormat;
        SQLiteStringFormat *_deleteFilterItemFormat;
        SQLiteStringFormat *_deleteCustomItemFormat;

        SQLiteStringFormat *_replaceLogFormat;
        SQLiteStringFormat *_replaceXmppUserFormat;
        SQLiteStringFormat *_replaceXmppMUCFormat;
        SQLiteStringFormat *_replaceCommandItemFormat;
        SQLiteStringFormat *_replaceFilterItemFormat;
        SQLiteStringFormat *_replaceCustomItemFormat;

        Database *_db;
        unsigned int _index;
        bool _writeable;
        std::string _dbFile;
};

#endif
