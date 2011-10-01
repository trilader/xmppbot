#ifndef SUBJECTBOTCOMMAND_H
#define SUBJECTBOTCOMMAND_H

#include "BotCommand.h"
#include "gloox/mucroom.h"

class SubjectBotCommand : public BotCommand
{
public:
    SubjectBotCommand(MUCRoom *room);
    void invoke(const JID& user, const std::string& args) const;

private:
    MUCRoom *_room;
};

#endif
