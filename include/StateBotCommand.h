#ifndef STATEBOTCOMMAND_H
#define STATEBOTCOMMAND_H

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/unordered_map.hpp"
#include "boost/algorithm/string.hpp"

#include "gloox/jid.h"
#include "gloox/mucroom.h"

#include "BotCommand.h"
#include "StringFormat.h"

class StateBotCommand : public BotCommand
{
public:
    StateBotCommand(MUCRoom *room);
    virtual ~StateBotCommand();
    bool invoke(const JID& user, const bool priv, const std::string& args, std::string *response) const;
    std::string getHelp() const;
    bool showHelp() const;

    void removeUserState(const JID& roomuser);
    void removeUserState(const std::string& nick);

private:
    bool parseArgs(const std::string& args, std::string *state, std::string *reason) const;

    MUCRoom *_room;
    boost::unordered_map<std::string, std::pair<boost::posix_time::ptime,std::string>> *_afkMap;
    StringFormat* _afkFormat;
    StringFormat* _reFormat;
};

#endif
