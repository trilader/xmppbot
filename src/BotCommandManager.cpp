#include "BotCommandManager.h"

BotCommandManager::BotCommandManager()
{
    this->_commandMap = new boost::unordered_map<std::string, BotCommand* >();
    //_commandMap = new std::map<std::string, BotCommand* >();}
}

void BotCommandManager::registerCommand(const std::string& name, BotCommand *command)
{
    if(0 == command)
        return;

    (*_commandMap)[name] = command;
}

void BotCommandManager::tryInvoke(const std::string& name, const std::string& args, const JID& user, bool *success)
{
    *success = this->isKnownCommand(name);

    if(*success)
        (*_commandMap)[name]->invoke(user, args);
}

void BotCommandManager::tryInvokeFromString(const std::string& str,const JID& user, bool *success)
{
    std::string name, args;

    BotCommandManager::splitCommandString(str, &name, &args);

    this->tryInvoke(name, args, user, success);
}

bool BotCommandManager::isKnownCommand(const std::string& name)
{
    return _commandMap->find(name) != _commandMap->end();
}

void BotCommandManager::splitCommandString(const std::string& str, std::string *name, std::string *args)
{
    std::string cpy = boost::trim_copy(str);

    std::size_t splitpos = cpy.find(" ");

    if(std::string::npos == splitpos)
    {
        *name = cpy;
        *args = "";
        return;
    }

    *name = cpy.substr(0,splitpos);
    *args = cpy.substr(splitpos + 1);
}
