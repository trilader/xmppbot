#ifndef BOTCOMMAND_H
#define BOTCOMMAND_H

#include <string>

#include "gloox/jid.h"

using namespace gloox;

class BotCommand
{
public:
    virtual bool invoke(const JID& user, const std::string& args, std::string *response) const = 0;
    virtual std::string getHelp() const = 0;
    virtual bool showHelp() const = 0;
};

#endif
