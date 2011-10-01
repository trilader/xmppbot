#ifndef BOTCOMMAND_H
#define BOTCOMMAND_H

#include <string>
#include "gloox/jid.h"

using namespace gloox;

class BotCommand
{
public:
    virtual void invoke(const JID& user, const std::string& args) const = 0;
};

#endif
