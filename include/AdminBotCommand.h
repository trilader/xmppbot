#ifndef ADMINBOTCOMMAND_H
#define ADMINBOTCOMMAND_H

#include "ProtectedBotCommand.h"
#include "gloox/client.h"

class AdminBotCommand : public ProtectedBotCommand
{
public:
    AdminBotCommand(std::string adminpw, Client* client);
    bool invoke(const JID& user, const std::string& args, std::string *response) const;

    std::string getHelp() const;
    bool showHelp() const;

private:
    bool quit(std::string *response) const;

    Client *_client;
};

#endif
