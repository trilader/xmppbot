#include "SubjectBotCommand.h"

SubjectBotCommand::SubjectBotCommand(MUCRoom *room)
{
    this->_room = room;
}

void SubjectBotCommand::invoke(const JID& user, const std::string& args) const
{
    this->_room->setSubject(args);
}
