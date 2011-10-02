#ifndef TESTBOTCOMMAND_H
#define TESTBOTCOMMAND_H

#include "BotCommand.h"
#include <iostream>

class TestBotCommand : public BotCommand
{
public:
    bool invoke(const JID& user, const std::string& args, std::string *response) const;
    std::string getHelp() const;
    bool showHelp() const;
};

#endif
