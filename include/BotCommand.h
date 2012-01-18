#ifndef BOTCOMMAND_H
#define BOTCOMMAND_H

#include <string>
#include <vector>

#include "boost/algorithm/string.hpp"
#include "gloox/jid.h"

#include "BotCommandInfo.h"
#include "Configuration.h"
#include "ConfigurationBased.h"

using namespace gloox;

class BotCommand : public ConfigurationBased
{
public:
    BotCommand(Configuration *config = 0);

    virtual bool invoke(BotCommandInfo *info) const = 0;
    virtual std::string getHelp() const = 0;
    virtual bool showHelp() const = 0;

protected:
    virtual std::string getOption(const std::string& name, const std::string& option) const;
};

#endif
