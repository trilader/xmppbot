#include "KickCommand.h"

KickCommand::KickCommand(Client* client, MUCRoom* room, std::string password)
{
    m_Client = client;
    m_Room = room;
    m_Pw = password;
}

bool KickCommand::invoke(const JID& user, const std::string& args, std::string *response) const
{
    if(m_Client==NULL)
        return false;
    if(m_Room==NULL)
        return false;

    //std::cout << "kick called by "<<user.full() << std::endl;
    //std::cout << "args: " << args << std::endl;

    unsigned int pos=args.find_first_of(" ");

    if(pos == std::string::npos)
    {
        m_Room->kick(user.resource(), "Selfkick.");
        return true;
    }

    std::string arg[2];

    arg[0]=args.substr(0,pos);
    arg[1]=args.substr(pos+1);

    //std::cout <<"arg[0]: \""<<arg[0]<<"\" arg[1]: \""<<arg[1] << "\""<< std::endl;

    if(arg[0]==m_Pw && arg[1]!="")
    {
        //std::cout << "admin kick" << std::endl;
        m_Room->kick(arg[1],"Kicked by "+user.resource());
        return true;
    }

    return false;
}
