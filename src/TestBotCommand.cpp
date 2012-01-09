#include "TestBotCommand.h"

bool TestBotCommand::invoke(const JID& user, const std::string& args, std::string *response) const
{
    std::cout << args << std::endl;
    *response = "You wrote: " + args;

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
