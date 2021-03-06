#ifndef ALIASBOTCOMMAND_H
#define ALIASBOTCOMMAND_H

#include "BotCommand.h"

class AliasBotCommand : public BotCommand
{
public:
    AliasBotCommand(std::string argsprefix, std::string argspostfix, std::string helptext, bool showInHelp, BotCommand *command);
    bool invoke(BotCommandInfo *info) const;
    std::string getHelp() const;
    bool showHelp() const;

private:
    BotCommand *_command;

    std::string _name;
    std::string _prefix;
    std::string _postfix;
    std::string _helptext;
    bool _showInHelp;
};

#endif
