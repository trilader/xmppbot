#include "SubjectBotCommand.h"

SubjectBotCommand::SubjectBotCommand(MUCRoom *room, std::string adminpw, StringFormat *format) : ProtectedBotCommand(adminpw)
{
    this->_room = room;
    this->_format = format;

    LOG(debug) << "Subject format: " + _format->getFormatString();
}

bool SubjectBotCommand::invoke(const JID& user, const bool priv, const std::string& args, std::string *response) const
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

    if(this->_eventenabled)
    {
        boost::gregorian::date_period period(now, this->_eventdate);

        this->_format->assign("1",period.length());
    }

    this->_format->assign("2", subject);

    std::string result = this->_format->produce();

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

void SubjectBotCommand::setEvent(std::string datestr)
{
    this->_eventdate = boost::gregorian::from_string(datestr);
    this->_eventenabled = true;
}
