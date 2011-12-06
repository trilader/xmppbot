#include "ForeignMessageFilter.h"

ForeignMessageFilter::ForeignMessageFilter(JIDMap *usermap,std::set<std::string> *authusers, Client *client)
{
    this->_usermap = usermap;
    this->_client = client;
    this->_authUsers = authusers;
}

void ForeignMessageFilter::handleMessage(const Message& stanza, bool room, bool priv, bool *handled)
{
    if(room)
        return;

    if(this->_usermap->count(stanza.from())>0)
        return;

    std::set<std::string>::iterator it =  this->_authUsers->find(stanza.from().bare());
    if(it!=_authUsers->end())
    {
        LOG(debug)<<"authalways: found "+(*it)+". request from: "+stanza.from().bare();
        return;
    }


    Message m(Message::Chat, stanza.from(),"I don't think you are in my room!");
    this->_client->send(m);

    *handled = true;
}
