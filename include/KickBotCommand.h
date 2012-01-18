#ifndef KICKCOMMAND_H
#define KICKCOMMAND_H

#include <iostream>

#include "gloox/client.h"
#include "gloox/mucroom.h"
#include "gloox/message.h"

#include "ProtectedBotCommand.h"
#include "Configuration.h"

class KickBotCommand : public ProtectedBotCommand
{
public:
    KickBotCommand(Client* client, MUCRoom* room, Configuration *config);
    bool invoke(BotCommandInfo *info) const;
    std::string getHelp() const;
    bool showHelp() const;

private:
    Client* m_Client;
    MUCRoom* m_Room;
};

#endif
