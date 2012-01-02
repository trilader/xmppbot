#ifndef FILECONFIGURATION_H
#define FILECONFIGURATION_H

#include "Configuration.h"
#include "boost/program_options.hpp"
#include <iostream>
#include <fstream>
#include "LogHelper.h"

namespace opt = boost::program_options;

class FileConfiguration : public Configuration
{
    public:
        FileConfiguration(std::string file);

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
        bool isItemSet(const std::string& option);
        std::string getItem(const std::string& option);
        void addLogOptions(const std::string& name, opt::options_description *desc);

        opt::variables_map vm;
        std::string cmd_prefix;
        std::string filter_prefix;
        std::string custom_prefix;
        std::string muc_prefix;
        std::string user_prefix;
        std::string log_prefix;
};

#endif
