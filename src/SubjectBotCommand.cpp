#include "SubjectBotCommand.h"

SubjectBotCommand::SubjectBotCommand(MUCRoom *room, std::string adminpw)
{
    this->_room = room;
    this->_adminpw = adminpw;
}

bool SubjectBotCommand::invoke(const JID& user, const std::string& args, std::string *response) const
{
    std::string adminpw, subject;
    bool success = this->parseArgs(args, &adminpw, &subject);
    if(!success)
    {
        *response = "expecting 2 arguments";
        return false;
    }

    if(adminpw != this->_adminpw)
    {
        *response = "wrong password";
        return false;
    }

    std::string prefix = "";
    std::cout << "event name is" << this->_eventname << std::endl;
    if(this->_eventname.length() > 0)
    {
        std::cout << "read event" << std::endl;
        boost::gregorian::date now(boost::gregorian::day_clock::local_day());
        boost::gregorian::date_period period(now, this->_eventdate);
        prefix = this->_eventname + " ( noch " + boost::lexical_cast<std::string>(period.length()) + " Tage ) - ";
    }

    this->_room->setSubject(prefix + subject);

    *response = "Subject set to \"" + prefix + subject + "\".";
    return true;
}

bool SubjectBotCommand::parseArgs(const std::string& args, std::string *adminpw, std::string *subject) const
{
    std::string cpy = boost::trim_copy(args);

    std::size_t splitpos = cpy.find(" ");

    if(std::string::npos == splitpos)
        return false;

    *adminpw = cpy.substr(0,splitpos);
    *subject = cpy.substr(splitpos + 1);

    return true;
}

void SubjectBotCommand::setEvent(std::string name, std::string datestr)
{
    this->_eventname = name;

    this->_eventdate = boost::gregorian::from_string(datestr);

    std::cout << "new event is " << this->_eventname << " and event date is " << this->_eventdate << std::endl;
}
