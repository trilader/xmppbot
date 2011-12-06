#include "ForeignMessageFilter.h"

ForeignMessageFilter::ForeignMessageFilter(JIDMap *usermap,std::set<gloox::JID> *authusers, Client *client)
{
    this->_usermap = usermap;
    this->_client = client;
    this->_authUsers = authusers;
}

void ForeignMessageFilter::handleMessage(const Message& stanza, bool room, bool priv, bool *handled)
{
    if(room)
        return;

    if(this->_usermap->count(stanza.from()))
        return;

    if(this->_authUsers->count(stanza.from()))
        return;

    Message m(Message::Chat, stanza.from(),"I don't think you are in my room!");
    this->_client->send(m);

    *handled = true;
}
