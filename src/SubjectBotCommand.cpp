#include "SubjectBotCommand.h"

SubjectBotCommand::SubjectBotCommand(MUCRoom *room, Configuration *config) : ProtectedBotCommand(config)
{
    this->_room = room;
    this->_defaultFormat = "%_";
}

bool SubjectBotCommand::invoke(BotCommandInfo *info) const
{
    BotCommandArgs args = info->parseArgs(2);

    if(args.size() < 1 || !this->checkPassword(args[0]))
    {
        info->setResponse("Wrong password");
        return false;
    }

    boost::gregorian::date now(boost::gregorian::day_clock::local_day());

    std::string formatstr = this->getOption("subject", "format");
    if("" == formatstr)
        formatstr = this->_defaultFormat;

    StringFormat format(formatstr);
    std::string datestr = this->getOption("subject", "eventdate");
    if(datestr.length() > 0)
    {
        std::vector<std::string> dates;
        boost::algorithm::split(dates,datestr,boost::algorithm::is_any_of(","));

        for(unsigned int i = 0; i < dates.size(); i++)
        {
            boost::algorithm::trim(dates[i]);
            if(dates[i].length() < 1)
                continue;

            boost::gregorian::date_period period(now, boost::gregorian::from_string(dates[i]));

            format.assign(boost::lexical_cast<std::string>(i+1),period.length());
        }
    }

    if(args.size()<2 && this->_defaultFormat==formatstr)
    {
        info->setResponse("Expecting custom subject");
        return false;
    }

    if(args.size()>=2)
    	format.assign("_", args[1]);
    else // don't display anything if there is nothing to display
        format.assign("_","");

    std::string result = format.produce();

    this->_room->setSubject(result);
    info->setResponse("Subject set to \"" + result + "\".");
    return true;
}

std::string SubjectBotCommand::getHelp() const
{
    return std::string("<password> [<subject>] - Set or update the rooms subject");
}

bool SubjectBotCommand::showHelp() const
{
    return true;
}
