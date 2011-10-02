#include "SubjectBotCommand.h"

SubjectBotCommand::SubjectBotCommand(MUCRoom *room, std::string adminpw, std::string format) : ProtectedBotCommand(adminpw)
{
    this->_room = room;
    this->_format = format;
    std::cout << "Format: "<<_format<<std::endl;
}

bool SubjectBotCommand::invoke(const JID& user, const std::string& args, std::string *response) const
{
    std::string subject;
    bool success = this->checkPassword(args, &subject);
    if("" == subject)
    {
        *response = "Expecting custom subject";
        return false;
    }

    if(!success)
    {
        *response = "Wrong password";
        return false;
    }

    boost::gregorian::date now(boost::gregorian::day_clock::local_day());
    boost::gregorian::date_period period(now, this->_eventdate);

    std::string result = _format.substr(0);

    if(this->_eventname.length() > 0)
        boost::algorithm::replace_all(result,"%1",this->_eventname);

    boost::algorithm::replace_all(result,"%2",boost::lexical_cast<std::string>(period.length()));
    boost::algorithm::replace_all(result,"%3",subject);

    this->_room->setSubject(result);
    *response = "Subject set to \"" + result + "\".";
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

    //std::cout << "new event is " << this->_eventname << " and event date is " << this->_eventdate << std::endl;
}
