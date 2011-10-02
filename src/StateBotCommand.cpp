#include "StateBotCommand.h"

StateBotCommand::StateBotCommand(MUCRoom *room)
{
    this->_room = room;
    this->_afkMap = new boost::unordered_map<std::string, std::pair<boost::posix_time::ptime,std::string>>();
}

bool StateBotCommand::invoke(const JID& user, const std::string& args, std::string *response) const
{
    std::string state, reason;
    bool success = this->parseArgs(args, &state, &reason);
    if(!success)
    {
        *response = "expecting 2 arguments";
        return false;
    }

    if(state != "afk" && state != "re")
    {
        *response = "unknown state";
        return false;
    }

    boost::posix_time::ptime t(boost::posix_time::second_clock::local_time());
    if(state == "afk")
    {
        std::string reason_str;
        if(reason.length() > 0)
            reason_str = " ( "+reason+" )";

        (*(this->_afkMap))[user.resource()] = std::pair<boost::posix_time::ptime,std::string>(t,reason_str);

        this->_room->send(user.resource() + " is afk" + reason_str + "!");
    }
    else
    {
        if(!this->_afkMap->count(user.resource()))
        {
            *response = "you are not afk...";
            return false;
        }

        boost::posix_time::time_period period((*(this->_afkMap))[user.resource()].first, t);
        std::string r = (*(this->_afkMap))[user.resource()].second;

        this->_afkMap->erase(user.resource());
        this->_room->send(user.resource() + " is back from " +r+" (" + boost::posix_time::to_simple_string(period.length()) + ")!");
    }

    return true;
}

std::string StateBotCommand::help() const
{
    return std::string("");
}

bool StateBotCommand::parseArgs(const std::string& args, std::string *state, std::string *reason) const
{
    std::string cpy = boost::trim_copy(args);

    std::size_t splitpos = cpy.find(" ");

    if(std::string::npos == splitpos)
    {
        *state = cpy;
        return true;
    }

    *state = cpy.substr(0,splitpos);
    *reason = cpy.substr(splitpos + 1);

    return true;
}
