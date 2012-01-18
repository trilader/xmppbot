#include "AdminBotCommand.h"

AdminBotCommand::AdminBotCommand(Configuration *config, XmppBot *bot) : ProtectedBotCommand(config)
{
    this->_bot = bot;
}

bool AdminBotCommand::showHelp() const
{
    return false;
}

std::string AdminBotCommand::getHelp() const
{
    return "";
}

bool AdminBotCommand::invoke(BotCommandInfo *info) const
{
    BotCommandArgs args = info->parseArgs(2);
    if(args.size() < 1 || !this->checkPassword(args[0]))
    {
        info->setResponse("wrong password");
        return false;
    }

    std::string cmd = (args.size() > 1) ? args[1] : "";

    if("quit" == cmd)
        return this->quit();

    if("reload" == cmd)
    	return this->reload();

    info->setResponse("unknown admin cmd");
    return false;
}

bool AdminBotCommand::quit() const
{
    this->_bot->quit();

    return true;
}

bool AdminBotCommand::reload() const
{
	this->_bot->quit(XmppBot::RELOAD_REQUESTED);

	return true;
}
