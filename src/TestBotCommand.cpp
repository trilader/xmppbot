#include "TestBotCommand.h"

bool TestBotCommand::invoke(BotCommandInfo *info) const
{
    //LOG(debug) << info->getArgumentString();
    info->setResponse("You wrote: " + info->getArgumentString());

    return true;
}

std::string TestBotCommand::getHelp() const
{
    return std::string("<message> - Just echo <message> back to the sender");
}

bool TestBotCommand::showHelp() const
{
    return false;
}
