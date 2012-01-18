#include "SubjectBotCommand.h"

SubjectBotCommand::SubjectBotCommand(MUCRoom *room, Configuration *config) : ProtectedBotCommand(config)
{
    this->_room = room;
}

bool SubjectBotCommand::invoke(BotCommandInfo *info) const
{
    BotCommandArgs args = info->parseArgs(2);

    if(args.size() < 1 || !this->checkPassword(args[0]))
    {
        info->setResponse("Wrong password");
        return false;
    }
    else if(args.size() < 2)
    {
        info->setResponse("Expecting custom subject");
        return false;
    }

    boost::gregorian::date now(boost::gregorian::day_clock::local_day());

    std::string formatstr = this->getOption("subject", "format");
    if("" == formatstr)
        formatstr = "%2";

    StringFormat format(formatstr);

    if(this->getConfig()->isCustomCommandItemSet("subject","eventdate"))
    {
        std::string datestr = this->getOption("subject", "eventdate");
        boost::gregorian::date_period period(now, boost::gregorian::from_string(datestr));

        format.assign("1",period.length());
    }

    format.assign("2", args[1]);

    std::string result = format.produce();

    this->_room->setSubject(result);
    info->setResponse("Subject set to \"" + result + "\".");
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
