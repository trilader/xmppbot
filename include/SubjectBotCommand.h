#ifndef SUBJECTBOTCOMMAND_H
#define SUBJECTBOTCOMMAND_H

#include "ProtectedBotCommand.h"
#include "gloox/mucroom.h"
#include "boost/algorithm/string/find.hpp"
#include "boost/algorithm/string/replace.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"
#include "StringFormat.h"

#include "LogHelper.h"

class SubjectBotCommand : public ProtectedBotCommand
{
public:
    SubjectBotCommand(MUCRoom *room, std::string adminpwm, StringFormat *format);
    bool invoke(const JID& user, const std::string& args, std::string *response) const;
    std::string getHelp() const;
    bool showHelp() const;

    void setEvent(std::string name, std::string datestr);

private:
    MUCRoom *_room;
    std::string _eventname;
    StringFormat *_format;
    boost::gregorian::date _eventdate;
};

#endif
