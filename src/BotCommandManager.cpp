#include "BotCommandManager.h"

BotCommandManager::BotCommandManager()
{
    this->_commandMap = new boost::unordered_map<std::string, BotCommand* >();
}

BotCommandManager::~BotCommandManager()
{
    for(boost::unordered_map<std::string, BotCommand*>::const_iterator it=_commandMap->begin(); it!=_commandMap->end(); it++)
        if(it->second!=NULL)
            delete it->second;
}

void BotCommandManager::registerCommand(const std::string& name, BotCommand *command)
{
    if(NULL == command)
        return;

    (*_commandMap)[name] = command;
}

bool BotCommandManager::tryInvoke(const std::string& name, const std::string& args, const JID& user, const bool priv, std::string *response)
{
    if(!this->isKnownCommand(name))
    {
        *response = "unknown command";

        return false;
    }

     return  (*_commandMap)[name]->invoke(user, priv, args, response);
}

bool BotCommandManager::tryInvokeFromString(const std::string& str,const JID& user, const bool priv, std::string *response)
{
    std::string name, args;

    BotCommandManager::splitCommandString(str, &name, &args);

    return this->tryInvoke(name, args, user, priv, response);
}

bool BotCommandManager::isKnownCommand(const std::string& name)
{
    return _commandMap->find(name) != _commandMap->end();
}

const boost::unordered_map<std::string, BotCommand*>* BotCommandManager::getCommands() const
{
    return (const boost::unordered_map<std::string, BotCommand*>*)_commandMap;
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
