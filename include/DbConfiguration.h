#ifndef DBCONFIGURATION_H
#define DBCONFIGURATION_H

#include "Configuration.h"
#include "DatabaseHelper.h"
#include "StringFormat.h"

class DbConfiguration : public Configuration
{
    public:

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

    protected:

        DbConfiguration(const soci::backend_factory& factory, std::string connString, bool writeable = true, unsigned int index = 0);

        virtual void initSQLStatements();
        virtual void initTables();
        virtual void openDatabase();
        virtual bool setOptionByFormat(StringFormat *format, const std::string& name, const std::string& option, const std::string& value);
        virtual bool getOptionByFormat(StringFormat *format, const std::string& name, const std::string& option, std::string *value);

        StringFormat *_tableFormat;

        StringFormat *_selectLogFormat;
        StringFormat *_selectXmppUserFormat;
        StringFormat *_selectXmppMUCFormat;
        StringFormat *_selectCommandItemFormat;
        StringFormat *_selectFilterItemFormat;
        StringFormat *_selectCustomItemFormat;

        StringFormat *_deleteLogFormat;
        StringFormat *_deleteXmppUserFormat;
        StringFormat *_deleteXmppMUCFormat;
        StringFormat *_deleteCommandItemFormat;
        StringFormat *_deleteFilterItemFormat;
        StringFormat *_deleteCustomItemFormat;

        StringFormat *_replaceLogFormat;
        StringFormat *_replaceXmppUserFormat;
        StringFormat *_replaceXmppMUCFormat;
        StringFormat *_replaceCommandItemFormat;
        StringFormat *_replaceFilterItemFormat;
        StringFormat *_replaceCustomItemFormat;

        soci::session *_db;
        const soci::backend_factory& _factory;
        unsigned int _index;
        bool _writeable;
        std::string _connString;
};

#endif

