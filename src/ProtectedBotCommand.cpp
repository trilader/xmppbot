#include "ProtectedBotCommand.h"

ProtectedBotCommand::ProtectedBotCommand(Configuration *config) : BotCommand(config)
{
}

bool ProtectedBotCommand::checkPassword(const std::string& password) const
{
    return password == this->getOption("admin", "password");
}
