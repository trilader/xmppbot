#ifndef ADMINBOTCOMMAND_H
#define ADMINBOTCOMMAND_H

#include "ProtectedBotCommand.h"
#include "gloox/client.h"
#include "XmppBot.h"

class AdminBotCommand : public ProtectedBotCommand
{
public:
    AdminBotCommand(Configuration *config, XmppBot* client);
    bool invoke(BotCommandInfo *info) const;

    std::string getHelp() const;
    bool showHelp() const;

private:
    bool quit() const;
    bool reload() const;

    XmppBot *_bot;
};

#endif
