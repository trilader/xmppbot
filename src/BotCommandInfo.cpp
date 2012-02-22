#include "BotCommandInfo.h"

BotCommandInfo::BotCommandInfo(std::string name, std::string args, MessageInfo *info)
{
    this->_argString = args;
    this->_name = name;
    this->_response = "";

    this->_private = info->isPrivate();
    this->_room = info->isRoom();

    this->_user = info->getFrom();
}

const std::string& BotCommandInfo::getName()
{
    return this->_name;
}

const std::string& BotCommandInfo::getArgumentString()
{
    return this->_argString;
}

const std::string& BotCommandInfo::getResponse()
{
    return this->_response;
}

const gloox::JID& BotCommandInfo::getUser()
{
    return this->_user;
}

bool BotCommandInfo::isPrivate()
{
    return this->_private;
}

bool BotCommandInfo::isRoom()
{
    return this->_room;
}

void BotCommandInfo::setArgumentString(const std::string& args)
{
    this->_argString = args;
}

void BotCommandInfo::setResponse(const std::string& response)
{
    this->_response = response;
}

void BotCommandInfo::setName(const std::string& name)
{
    this->_name = name;
}

BotCommandArgs BotCommandInfo::parseArgs(unsigned int max, std::string seperator)
{
    BotCommandArgs args;
    boost::algorithm::split(args, this->_argString, boost::is_any_of(seperator));

    if(max < 1)
        return args;

    while(args.size() > max)
    {
        args[args.size() - 2] = args[args.size() - 2] + seperator + args[args.size() - 1];
        args.pop_back();
    }

    return args;
}
