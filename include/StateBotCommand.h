#ifndef STATEBOTCOMMAND_H
#define STATEBOTCOMMAND_H

#include "BotCommand.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "gloox/jid.h"

class StateBotCommand : public BotCommand
{
public:
    AfkBotCommand(MUCRoom *room);
    bool invoke(const JID& user, const std::string& args, std::string *response) const;

private:
    MUCRoom *_room;
    boost::unordered_map<JID, boost::posix_time::ptime > *_afkMap;
};

#endif
