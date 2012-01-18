#include "BotCommand.h"

BotCommand::BotCommand(Configuration *config) : ConfigurationBased(config)
{
}

std::string BotCommand::getOption(const std::string& name, const std::string& option) const
{
    if(0 == this->getConfig())
        return "";

    return this->getConfig()->getCustomCommandItem(name, option);
}
