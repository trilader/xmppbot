#include "ForeignMessageFilter.h"

ForeignMessageFilter::ForeignMessageFilter(JIDMap *usermap,std::set<std::string> *authusers, Client *client)
{
    this->_usermap = usermap;
    this->_client = client;
    this->_authUsers = authusers;
}

void ForeignMessageFilter::handleMessage(MessageInfo *info)
{
    if(info->isRoom())
        return;

    if(this->_usermap->count(info->getFrom())>0)
        return;

    std::set<std::string>::iterator it =  this->_authUsers->find(info->getFrom().bare());
    if(it!=_authUsers->end())
    {
        LOG(debug)<<"authalways: found "+(*it)+". request from: "+info->getFrom().bare();
        return;
    }


    Message m(Message::Chat, info->getFrom(),"I don't think you are in my room!");
    this->_client->send(m);

    info->markHandled();
}
