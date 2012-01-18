#include "JIDMessageFilter.h"

JIDMessageFilter::JIDMessageFilter(const JID& jid, bool cmpBareOnly, bool room, bool priv)
{
    LOG(debug) << jid.username();

    if(cmpBareOnly)
        this->_jid = new JID(jid.bare());
    else
        this->_jid = new JID(jid);

    LOG(debug) << jid.username();
    this->_cmpBareOnly = cmpBareOnly;

    if(this->_cmpBareOnly)
        *(this->_jid) = this->_jid->bare();

    this->_room = room;
    this->_priv = priv;
}

void JIDMessageFilter::handleMessage(MessageInfo *info)
{
    if(info->isRoom() && !this->_room)
        return;

    if(info->isPrivate() && !this->_priv)
        return;

    JID from = info->getMessage().from();
    if(this->_cmpBareOnly)
        from = from.bare();

    if(*(this->_jid) == from)
        info->markHandled();
}
