#include "KickBotCommand.h"

KickBotCommand::KickBotCommand(Client* client, MUCRoom* room, std::string password): ProtectedBotCommand(password)
{
    m_Client = client;
    m_Room = room;
}

bool KickBotCommand::invoke(const JID& user, const std::string& args, std::string *response) const
{
    if(m_Client==NULL)
        return false;
    if(m_Room==NULL)
        return false;

    size_t pos=args.find_first_of(" ");

    if(std::string::npos == pos)
    {
        m_Room->kick(user.resource(), "Selfkick.");
        return true;
    }

    std::string cmd;
    if(!this->checkPassword(args, &cmd))
    {
        *response = "wrong password";
        return false;
    }

    if(""==cmd)
    {
        *response = "No user given";
        return false;
    }

    m_Room->kick(cmd,"Kicked by "+user.resource());
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
