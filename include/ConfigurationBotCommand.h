#ifndef CONFIGURATIONBOTCOMMAND_H
#define CONFIGURATIONBOTCOMMAND_H

#include "ProtectedBotCommand.h"
#include "Configuration.h"
#include "boost/lexical_cast.hpp"
#include "LogHelper.h"

class ConfigurationBotCommand : public ProtectedBotCommand
{
    public:
        ConfigurationBotCommand(Configuration *config);

        virtual bool invoke(BotCommandInfo *info) const;
        virtual std::string getHelp() const;
        virtual bool showHelp() const;
};

#endif
