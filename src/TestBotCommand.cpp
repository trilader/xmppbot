#include "TestBotCommand.h"

void TestBotCommand::invoke(const JID& user, const std::string& args) const
{
    std::cout << args << std::endl;
}
