#ifndef KICKCOMMAND_H
#define KICKCOMMAND_H

#include <iostream>

#include "gloox/client.h"
#include "gloox/mucroom.h"
#include "gloox/message.h"

#include "BotCommand.h"

class KickBotCommand : public BotCommand
{
public:
    KickBotCommand(Client* client, MUCRoom* room, std::string password);
    bool invoke(const JID& user, const std::string& args, std::string *response) const;
    std::string help() const;

private:
    Client* m_Client;
    MUCRoom* m_Room;
    std::string m_Pw;
};

#endif
