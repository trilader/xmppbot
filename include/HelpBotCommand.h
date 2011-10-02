#ifndef HELPBOTCOMMAND_H
#define HELPBOTCOMMAND_H

#include <map>

#include "BotCommand.h"

class HelpBotCommand: public BotCommand
{
    public:
        HelpBotCommand();
        bool invoke(const JID& user, const std::string& args, std::string *response) const;
        std::string help() const;
    protected:
    private:
};

#endif // HELPBOTCOMMAND_H
