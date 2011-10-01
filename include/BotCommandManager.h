#ifndef BOTCOMMANDMANAGER_H
#define BOTCOMMANDMANAGER_H

#include <string>
#include <map>
#include "boost/algorithm/string.hpp"
#include "boost/unordered_map.hpp"
#include "boost/algorithm/string/find.hpp"
#include "gloox/jid.h"
#include "BotCommand.h"

using namespace gloox;

class BotCommandManager
{
public:
    BotCommandManager();

    void registerCommand(const std::string& name, BotCommand *cmd);
    void tryInvokeFromString(const std::string& str,const JID& user, bool *success);
    void tryInvoke(const std::string& name, const std::string& args, const JID& user, bool *success);
    bool isKnownCommand(const std::string& name);

    static void splitCommandString(const std::string& str, std::string *name, std::string *args);

private:
    boost::unordered_map<std::string, BotCommand* > *_commandMap;
};

#endif
