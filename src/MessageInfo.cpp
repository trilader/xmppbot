#include "MessageInfo.h"

MessageInfo::MessageInfo(const gloox::Message& msg, bool room, bool priv)
{
    this->_room = room;
    this->_private = priv;
    this->_handled = false;
    this->_body = msg.body();
    this->_from = msg.from();
    this->_when = msg.when();
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

std::string MessageInfo::getBody()
{
    return this->_body;
}

void MessageInfo::setBody(const std::string& body)
{
    this->_body = body;
}

gloox::JID MessageInfo::getFrom()
{
    return this->_from;
}

void MessageInfo::setFrom(const gloox::JID& jid)
{
    this->_from = jid;
}

const gloox::DelayedDelivery* MessageInfo::getWhen()
{
    return this->_when;
}
