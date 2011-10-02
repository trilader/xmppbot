#ifndef ALIASBOTCOMMAND_H
#define ALIASBOTCOMMAND_H

#include "BotCommand.h"

class AliasBotCommand : public BotCommand
{
public:
    AliasBotCommand(std::string argsprefix, std::string argspostfix, BotCommand *command);
    bool invoke(const JID& user, const std::string& args, std::string *response) const;
    std::string help() const;

private:
    BotCommand *_command;
    std::string _prefix;
    std::string _postfix;
};

#endif
