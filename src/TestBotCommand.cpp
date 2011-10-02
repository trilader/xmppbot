#include "TestBotCommand.h"

bool TestBotCommand::invoke(const JID& user, const std::string& args, std::string *response) const
{
    std::cout << args << std::endl;
    *response = "You wrote: " + args;

    return true;
}

std::string TestBotCommand::help() const
{
    return std::string("");
}
