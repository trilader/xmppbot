#include "AliasBotCommand.h"

AliasBotCommand::AliasBotCommand(std::string prefix, std::string postfix, std::string helptext, bool showInHelp, BotCommand *command)
{
    this->_command = command;
    this->_prefix = prefix;
    this->_postfix = postfix;
    this->_helptext = helptext;
    this->_showInHelp = showInHelp;
}

bool AliasBotCommand::invoke(const JID& user, const bool priv, const std::string& args, std::string *response) const
{
    return this->_command->invoke(user, priv, this->_prefix + args + this->_postfix, response);
}

std::string AliasBotCommand::getHelp() const
{
    return _helptext;
}

bool AliasBotCommand::showHelp() const
{
    return _showInHelp;
}
