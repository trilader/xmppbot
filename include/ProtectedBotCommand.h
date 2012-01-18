#ifndef PROTECTEDBOTCOMMAND_H
#define PROTECTEDBOTCOMMAND_H

#include "BotCommand.h"
#include "boost/algorithm/string/find.hpp"
#include "boost/algorithm/string.hpp"

class ProtectedBotCommand : public BotCommand
{
protected:
    ProtectedBotCommand(Configuration *config);
    bool checkPassword(const std::string& password) const;
};

#endif
