#include "SubjectBotCommand.h"

SubjectBotCommand::SubjectBotCommand(MUCRoom *room, std::string adminpw) : ProtectedBotCommand(adminpw)
{
    this->_room = room;
}

bool SubjectBotCommand::invoke(const JID& user, const std::string& args, std::string *response) const
{
    std::string subject;
    bool success = this->checkPassword(args, &subject);
    if("" == subject)
    {
        *response = "expecting custom subject";
        return false;
    }

    if(!success)
    {
        *response = "wrong password";
        return false;
    }

    std::string prefix = "";
    //std::cout << "event name is" << this->_eventname << std::endl;
    if(this->_eventname.length() > 0)
    {
        //std::cout << "read event" << std::endl;
        boost::gregorian::date now(boost::gregorian::day_clock::local_day());
        boost::gregorian::date_period period(now, this->_eventdate);
        prefix = this->_eventname + " ( noch " + boost::lexical_cast<std::string>(period.length()) + " Tage ) - ";
    }

    this->_room->setSubject(prefix + subject);

    *response = "Subject set to \"" + prefix + subject + "\".";
    return true;
}

std::string SubjectBotCommand::getHelp() const
{
    return std::string("<password> <subject> - Set the rooms description");
}

bool SubjectBotCommand::showHelp() const
{
    return true;
}

void SubjectBotCommand::setEvent(std::string name, std::string datestr)
{
    this->_eventname = name;

    this->_eventdate = boost::gregorian::from_string(datestr);

    std::cout << "new event is " << this->_eventname << " and event date is " << this->_eventdate << std::endl;
}
