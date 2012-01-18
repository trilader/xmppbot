#ifndef BOTCOMMANDINFO_H
#define BOTCOMMANDINFO_H

#include <string>
#include <vector>
#include "gloox/client.h"
#include "boost/algorithm/string.hpp"

#include "MessageInfo.h"

typedef std::vector<std::string> BotCommandArgs;

class BotCommandInfo
{
    public:
        BotCommandInfo(std::string name, std::string args, MessageInfo *info);

        bool isPrivate();
        bool isRoom();

        const gloox::JID& getUser();
        const std::string& getName();
        const std::string& getArgumentString();
        const std::string& getResponse();

        void setArgumentString(const std::string& args);
        void setResponse(const std::string& response);
        void setName(const std::string& name);

        BotCommandArgs parseArgs(unsigned int max = -1, std::string seperator = " ");

    private:
        std::string _name;
        std::string _argString;
        std::string _response;

        gloox::JID _user;

        bool _room;
        bool _private;
};

#endif
