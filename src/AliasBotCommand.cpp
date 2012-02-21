#include "AliasBotCommand.h"

AliasBotCommand::AliasBotCommand(std::string prefix, std::string postfix, std::string helptext, bool showInHelp, BotCommand *command)
{
    this->_command = command;
    this->_prefix = prefix;
    this->_postfix = postfix;
    this->_helptext = helptext;
    this->_showInHelp = showInHelp;
}

bool AliasBotCommand::invoke(BotCommandInfo *info) const
{
    info->setArgumentString(this->_prefix + info->getArgumentString() + this->_postfix);

    return this->_command->invoke(info);
}

std::string AliasBotCommand::getHelp() const
{
    return _helptext;
}

bool AliasBotCommand::showHelp() const
{
    return _showInHelp;
}
