#include "ProtectedBotCommand.h"

ProtectedBotCommand::ProtectedBotCommand(std::string adminpw)
{
    this->_adminpw = adminpw;
}

bool ProtectedBotCommand::checkPassword(const std::string& args_in, std::string *args_out) const
{
    std::string adminpw, left;
    this->parseArgs(args_in, &adminpw, &left);

    *args_out = left;

    return (adminpw == this->_adminpw);
}

void ProtectedBotCommand::parseArgs(const std::string& args, std::string *adminpw, std::string *left) const
{
    std::string cpy = boost::trim_copy(args);

    std::size_t splitpos = cpy.find(" ");

    if(std::string::npos == splitpos)
    {
        *adminpw = cpy;
        *left = "";
    }

    *adminpw = cpy.substr(0,splitpos);
    *left = cpy.substr(splitpos + 1);
}
