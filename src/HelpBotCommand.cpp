#include "HelpBotCommand.h"

HelpBotCommand::HelpBotCommand()
{
    //ctor
}

bool HelpBotCommand::invoke(const JID& user, const std::string& args, std::string *response) const
{
    return true;
}

std::string HelpBotCommand::help() const
{
    return std::string("Get help and usage information");
}
