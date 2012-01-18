#include "StateBotCommand.h"

StateBotCommand::StateBotCommand(MUCRoom *room)
{
    this->_room = room;
    this->_afkMap = new boost::unordered_map<std::string, std::pair<boost::posix_time::ptime,std::string>>();
    this->_afkFormat = new StringFormat("%user is now away from keyboard %reason");
    this->_reFormat = new StringFormat("%user is back %reason (%time)");
}

StateBotCommand::~StateBotCommand()
{
    delete _afkMap;
    delete _afkFormat;
    delete _reFormat;
}

bool StateBotCommand::invoke(BotCommandInfo *info) const
{
    std::string state, reason;
    BotCommandArgs args = info->parseArgs(2);
    if(args.size() < 1)
    {
        info->setResponse("expecting 2 arguments");
        return false;
    }

    state = args[0];
    reason = (args.size() > 1) ? args[1] : "";

    if(state != "afk" && state != "re" && state != "of")
    {
        info->setResponse("unknown state");
        return false;
    }

    boost::posix_time::ptime t(boost::posix_time::second_clock::local_time());
    if(state == "afk")
    {
        std::string reason_str = "";
        if(reason.length() > 0)
            reason_str = reason;

        _afkFormat->assign("user", info->getUser().resource());
        _afkFormat->assign("reason","("+reason_str+")");

        (*(this->_afkMap))[info->getUser().resource()] = std::pair<boost::posix_time::ptime,std::string>(t,reason_str);

        this->_room->send(_afkFormat->produce());
    }
    else if(state == "re")
    {
        if(!this->_afkMap->count(info->getUser().resource()))
        {
            info->setResponse("You are not away...");
            return false;
        }

        boost::posix_time::time_period period((*(this->_afkMap))[info->getUser().resource()].first, t);
        std::string r = (*(this->_afkMap))[info->getUser().resource()].second;

        _reFormat->assign("user",info->getUser().resource());
        _reFormat->assign("reason",r.length()>0?"from "+r:"");
        _reFormat->assign("time",boost::posix_time::to_simple_string(period.length()));

        this->_afkMap->erase(info->getUser().resource());
        this->_room->send(_reFormat->produce());
    }
    else //state == "of"
    {
        std::string msg;
        std::string user = reason;

        msg = info->getUser()+" is ";

        if(!this->_afkMap->count(info->getUser().resource()))
        {
            msg += "not away or not in the room";
        }
        else
        {
            boost::posix_time::time_period period((*(this->_afkMap))[info->getUser().resource()].first, t);
            std::string r = (*(this->_afkMap))[info->getUser().resource()].second;
            msg += "away since: "+boost::posix_time::to_simple_string(period.length());

            if(r.length()>0) msg+=" ("+r+")";
        }

        if (info->isPrivate())
            info->setResponse(msg);
        else
            this->_room->send(msg);
    }

    return true;
}

std::string StateBotCommand::getHelp() const
{
    return std::string("<state> [<reason>] - State can be 'afk', 're' or 'of'. If the state is 'afk' you can specify an optional message for the room. With 'of' you can query the state of a user");
}

bool StateBotCommand::showHelp() const
{
    return true;
}

void StateBotCommand::removeUserState(const JID& user)
{
    this->removeUserState(user.resource());
}

void StateBotCommand::removeUserState(const std::string& nick)
{
    if(this->_afkMap->count(nick))
        this->_afkMap->erase(nick);
}
