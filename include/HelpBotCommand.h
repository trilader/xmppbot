#ifndef HELPBOTCOMMAND_H
#define HELPBOTCOMMAND_H

#include <sstream>

#include "boost/unordered_map.hpp"

#include "BotCommand.h"

class HelpBotCommand: public BotCommand
{
    public:
        HelpBotCommand(const boost::unordered_map<std::string, BotCommand*>* commandList);
        bool invoke(const JID& user, const std::string& args, std::string *response) const;
        std::string getHelp() const;
        bool showHelp() const;
    protected:
    private:
        const boost::unordered_map<std::string, BotCommand*>* m_CommandList;
};

#endif // HELPBOTCOMMAND_H
