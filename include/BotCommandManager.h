#ifndef BOTCOMMANDMANAGER_H
#define BOTCOMMANDMANAGER_H

#include <string>
#include <map>
#include <vector>
#include "boost/algorithm/string.hpp"
#include "boost/unordered_map.hpp"
#include "boost/algorithm/string/find.hpp"
#include "gloox/jid.h"
#include "BotCommand.h"
#include "MessageInfo.h"
#include "BotCommandInfo.h"

using namespace gloox;

class BotCommandManager
{
public:
    BotCommandManager();
    ~BotCommandManager();

    void registerCommand(const std::string& name, BotCommand *cmd);
    bool tryInvoke(MessageInfo *info, std::string *response);
    bool tryInvoke(BotCommandInfo *info);
    bool isKnownCommand(const std::string& name);
    const boost::unordered_map<std::string, BotCommand*>* getCommands() const;

    static void splitCommandString(const std::string& str, std::string *name, std::string *args);

private:
    boost::unordered_map<std::string, BotCommand*>* _commandMap;
};

#endif
