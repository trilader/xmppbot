#include "KickBotCommand.h"

KickBotCommand::KickBotCommand(Client* client, MUCRoom* room, std::string password)
{
    m_Client = client;
    m_Room = room;
    m_Pw = password;
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

    std::string arg[2];

    arg[0]=args.substr(0,pos);
    arg[1]=args.substr(pos+1);

    if(arg[0]!=m_Pw)
    {
        *response = "Invalid password";
        return false;
    }
    else if(arg[1]=="")
    {
        *response = "No user given";
        return false;
    }

    m_Room->kick(arg[1],"Kicked by "+user.resource());
    return true;
}

std::string KickBotCommand::getHelp() const
{
    return std::string("<password> <nickname> - Kick the <nickname> from the room. Without parameters this is a self-kick");
}

bool KickBotCommand::showHelp() const
{
    return true;
}
