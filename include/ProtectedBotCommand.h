#ifndef PROTECTEDBOTCOMMAND_H
#define PROTECTEDBOTCOMMAND_H

#include "BotCommand.h"
#include "boost/algorithm/string/find.hpp"
#include "boost/algorithm/string.hpp"

class ProtectedBotCommand : public BotCommand
{
protected:
    ProtectedBotCommand(std::string adminpw);
    bool checkPassword(const std::string& args_in, std::string *args_out) const;

private:
    void parseArgs(const std::string& args, std::string *adminpw, std::string *subject) const;
    std::string _adminpw;
};

#endif
