#ifndef ADMINBOTCOMMAND_H
#define ADMINBOTCOMMAND_H

#include "ProtectedBotCommand.h"
#include "gloox/client.h"
#include "XmppBot.h"

class AdminBotCommand : public ProtectedBotCommand
{
public:
    AdminBotCommand(std::string adminpw, XmppBot* client);
    bool invoke(const JID& user, const std::string& args, std::string *response) const;

    std::string getHelp() const;
    bool showHelp() const;

private:
    bool quit(std::string *response) const;
    bool reload(std::string *response) const;

    XmppBot *_bot;
};

#endif
