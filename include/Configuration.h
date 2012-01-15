#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <vector>
#include "boost/algorithm/string.hpp"

class Configuration
{
    public:
        virtual bool setConfigurationIndex(unsigned int index) = 0;
        virtual unsigned int getConfigurationIndex() = 0;

        virtual bool isWritable() = 0;

        //Logs
        virtual bool setLog(const std::string& name, const std::string& type, bool enabled = true, bool keepopen = false) = 0;
        virtual bool setLog(const std::string& name, const std::string& type, const std::string& fileformat, const std::string& entryformat, bool enabled = true, bool keepopen = false) = 0;
        virtual bool getLog(const std::string& name, std::string *type, bool *enabled, bool *keepopen) = 0;
        virtual bool getLog(const std::string& name, std::string *type, std::string *fileformat, std::string *entryformat, bool *enabled, bool *keepopen) = 0;

        //Custom
        virtual bool setCustomItem(const std::string& name, const std::string& value) = 0;
        virtual std::string getCustomItem(const std::string& name) = 0;
        virtual bool isCustomItemSet(const std::string& name) = 0;

        //Commands
        virtual bool setCustomCommandItem(const std::string& cmd, const std::string& name, const std::string& value) = 0;
        virtual std::string getCustomCommandItem(const std::string& cmd, const std::string& name) = 0;
        virtual bool isCustomCommandItemSet(const std::string& cmd, const std::string& name) = 0;

        //Filter
        virtual bool setCustomFilterItem(const std::string& filter, const std::string& name, const std::string& value) = 0;
        virtual std::string getCustomFilterItem(const std::string& filter, const std::string& name) = 0;
        virtual bool isCustomFilterItemSet(const std::string& filter, const std::string& name) = 0;

        //Xmpp
        virtual bool setXmppUser(const std::string& user, const std::string& password, const std::string& address, const std::string& resource) = 0;
        virtual bool getXmppUser(std::string *user, std::string *password, std::string *address, std::string *resource) = 0;
        virtual bool setXmppMUC(const std::string& name, const std::string& room, const std::string& server) = 0;
        virtual bool getXmppMUC(std::string *name, std::string *room, std::string *server) = 0;

        bool setFromSeperatedString(const std::string& optionstr, const std::string& value, std::string seperator = ".");
        bool getFromSeperatedString(const std::string& optionstr, std::string *value, std::string seperator = ".");

        const std::string getMUCOptionIdent();
        const std::string getUserOptionIdent();
        const std::string getCommandOptionIdent();
        const std::string getFilterOptionIdent();
        const std::string getCustomOptionIdent();
        const std::string getLogOptionIdent();
};

inline const std::string Configuration::getMUCOptionIdent() { return "muc"; }
inline const std::string Configuration::getUserOptionIdent() { return "server"; }
inline const std::string Configuration::getCommandOptionIdent() { return "command"; }
inline const std::string Configuration::getFilterOptionIdent() { return "filter"; }
inline const std::string Configuration::getCustomOptionIdent() { return "bot"; }
inline const std::string Configuration::getLogOptionIdent() { return "logs"; }

#endif
