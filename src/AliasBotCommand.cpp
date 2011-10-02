#include "AliasBotCommand.h"

AliasBotCommand::AliasBotCommand(std::string prefix, std::string postfix, BotCommand *command)
{
    this->_command = command;
    this->_prefix = prefix;
    this->_postfix = postfix;
}

bool AliasBotCommand::invoke(const JID& user, const std::string& args, std::string *response) const
{
    return this->_command->invoke(user, this->_prefix + args + this->_postfix, response);
}

std::string AliasBotCommand::help() const
{
    return std::string("");
}
