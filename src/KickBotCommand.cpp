#include "KickBotCommand.h"

KickBotCommand::KickBotCommand(Client* client, MUCRoom* room, Configuration *config): ProtectedBotCommand(config)
{
    m_Client = client;
    m_Room = room;
}

bool KickBotCommand::invoke(BotCommandInfo *info) const
{
    if(m_Client==NULL)
        return false;
    if(m_Room==NULL)
        return false;

    BotCommandArgs args = info->parseArgs(2);

    if(args.size() < 2)
    {
        m_Room->kick(info->getUser().resource(), "Selfkick.");
        return true;
    }

    if(!this->checkPassword(args[0]))
    {
        info->setResponse("wrong password");
        return false;
    }

    if(""==args[1])
    {
        info->setResponse("No user given");
        return false;
    }

    m_Room->kick(args[1],"Kicked by "+info->getUser().resource());
    return true;
}

std::string KickBotCommand::getHelp() const
{
    return std::string("<password> <nickname> - Kick <nickname> from the room. Without parameters this is a self-kick");
}

bool KickBotCommand::showHelp() const
{
    return true;
}
