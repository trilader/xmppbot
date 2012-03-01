#ifndef SUBJECTBOTCOMMAND_H
#define SUBJECTBOTCOMMAND_H

#include "ProtectedBotCommand.h"
#include "gloox/mucroom.h"
#include "boost/algorithm/string/find.hpp"
#include "boost/algorithm/string/replace.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"
#include "StringFormat.h"

#include "LogHelper.h"

#include <vector>

class SubjectBotCommand : public ProtectedBotCommand
{
public:
    SubjectBotCommand(MUCRoom *room, Configuration *config);
    bool invoke(BotCommandInfo *info) const;
    std::string getHelp() const;
    bool showHelp() const;

private:
    MUCRoom *_room;
    std::string _defaultFormat;
};

#endif
