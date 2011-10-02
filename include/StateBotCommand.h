#ifndef STATEBOTCOMMAND_H
#define STATEBOTCOMMAND_H

#include "BotCommand.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "gloox/jid.h"
#include "gloox/mucroom.h"
#include "boost/unordered_map.hpp"
#include "boost/algorithm/string.hpp"

class StateBotCommand : public BotCommand
{
public:
    StateBotCommand(MUCRoom *room);
    bool invoke(const JID& user, const std::string& args, std::string *response) const;

private:
    bool parseArgs(const std::string& args, std::string *state, std::string *reason) const;

    MUCRoom *_room;
    boost::unordered_map<std::string, boost::posix_time::ptime > *_afkMap;
};

#endif