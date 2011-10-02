#ifndef KICKCOMMAND_H
#define KICKCOMMAND_H

#include <iostream>

#include "gloox/client.h"
#include "gloox/mucroom.h"
#include "gloox/message.h"

#include "BotCommand.h"

class KickCommand : public BotCommand
{
public:
    KickCommand(Client* client, MUCRoom* room, std::string password);
    bool invoke(const JID& user, const std::string& args, std::string *response) const;

private:
    Client* m_Client;
    MUCRoom* m_Room;
    std::string m_Pw;
};

#endif
