#ifndef TESTBOTCOMMAND_H
#define TESTBOTCOMMAND_H

#include "BotCommand.h"
#include <iostream>

class TestBotCommand : public BotCommand
{
public:
    void invoke(const JID& user, const std::string& args) const;
};

#endif
