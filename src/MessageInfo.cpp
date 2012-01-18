#include "MessageInfo.h"

MessageInfo::MessageInfo(const gloox::Message& msg, bool room, bool priv) : _message(msg)
{
    this->_room = room;
    this->_private = priv;
    this->_handled = false;
}

const gloox::Message& MessageInfo::getMessage()
{
    return this->_message;
}

bool MessageInfo::isPrivate()
{
    return this->_private;
}

bool MessageInfo::isRoom()
{
    return this->_room;
}

bool MessageInfo::isHandled()
{
    return this->_handled;
}

void MessageInfo::markHandled()
{
    this->_handled = true;
}
