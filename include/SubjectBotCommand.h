#ifndef SUBJECTBOTCOMMAND_H
#define SUBJECTBOTCOMMAND_H

#include "BotCommand.h"
#include "gloox/mucroom.h"
#include "boost/algorithm/string/find.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"

class SubjectBotCommand : public BotCommand
{
public:
    SubjectBotCommand(MUCRoom *room, std::string adminpw);
    bool invoke(const JID& user, const std::string& args, std::string *response) const;
    std::string getHelp() const;
    bool showHelp() const;

    void setEvent(std::string name, std::string datestr);

private:
    bool parseArgs(const std::string& args, std::string *adminpw, std::string *subject) const;

    MUCRoom *_room;
    std::string _adminpw;
    std::string _eventname;
    boost::gregorian::date _eventdate;
};

#endif
