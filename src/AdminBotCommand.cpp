#include "AdminBotCommand.h"

AdminBotCommand::AdminBotCommand(std::string adminpw, Client *client) : ProtectedBotCommand(adminpw)
{
    this->_client = client;
}

bool AdminBotCommand::showHelp() const
{
    return false;
}

std::string AdminBotCommand::getHelp() const
{
    return "";
}

bool AdminBotCommand::invoke(const JID& user, const std::string& args, std::string *response) const
{
    std::string cmd;
    if(!this->checkPassword(args, &cmd))
    {
        *response = "wrong password";
        return false;
    }

    if("quit" == cmd)
        return this->quit(response);

    *response = "unknown admin cmd";
    return false;
}

bool AdminBotCommand::quit(std::string *response) const
{
    *response = "";
    //std::cout << "disconnecting..." << std::endl;
    this->_client->disconnect();

    return true;
}
