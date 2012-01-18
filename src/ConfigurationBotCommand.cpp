#include "ConfigurationBotCommand.h"

ConfigurationBotCommand::ConfigurationBotCommand(Configuration *config) : ProtectedBotCommand(config)
{}

bool ConfigurationBotCommand::invoke(BotCommandInfo *info) const
{
    BotCommandArgs args = info->parseArgs(4);

    if(args.size() < 3)
    {
        info->setResponse("Expecting at least three arguments....");
        return false;
    }

    if(!this->checkPassword(args[0]))
    {
        info->setResponse("wrong password");
        return false;
    }

    if("get" == args[1])
    {
        std::string value;
        bool success = this->getConfig()->getFromSeperatedString(args[2], &value);

        if(!success)
        {
            info->setResponse("Failed to fetch option!");
            return false;
        }

        info->setResponse("Value is: " + value);
        return true;
    }

    if("set" == args[1])
    {
        if(!this->getConfig()->isWritable())
        {
            info->setResponse("Sorry, configuration is not writeable....");
            return true;
        }

        if(args.size() < 4)
        {
            info->setResponse("No value given!");
            return false;
        }

        bool success = this->getConfig()->setFromSeperatedString(args[2], args[3]);

        if(!success)
        {
            info->setResponse("Failed to set option!");
            return false;
        }

        info->setResponse("Success");
        return true;
    }

    if("index" == args[1])
    {
        if(args.size() > 3)
        {
            //WARNING; this can crash the bot
            try
            {
                this->getConfig()->setConfigurationIndex(boost::lexical_cast<int>(args[4]));
            }
            catch(...)
            {
                LOG(sys) << "Failed to cast configuration index given by xmpp command.";
            }
        }

        info->setResponse(std::string("Configuration index is ") + boost::lexical_cast<std::string>(this->getConfig()->getConfigurationIndex()) + ".");
        return true;
    }

    info->setResponse("unknown command option.");
    return false;
}

std::string ConfigurationBotCommand::getHelp() const
{
    return "";
}

bool ConfigurationBotCommand::showHelp() const
{
    return false;
}
