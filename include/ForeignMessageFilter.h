#ifndef FOREIGNMESSAGEHANDLER_H
#define FOREIGNMESSAGEHANDLER_H

#include "MessageFilter.h"
#include "gloox/client.h"
#include <set>
#include "JIDMap.h"

class ForeignMessageFilter : public MessageFilter
{
public:
    ForeignMessageFilter(JIDMap* usermap, std::set<gloox::JID> *authusers, Client *client);

    void handleMessage(const Message& stanza, bool room, bool priv, bool *handled);

private:
    JIDMap* _usermap;
    std::set<gloox::JID> *_authUsers;
    Client *_client;
};

#endif
