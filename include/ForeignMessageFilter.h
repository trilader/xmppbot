#ifndef FOREIGNMESSAGEHANDLER_H
#define FOREIGNMESSAGEHANDLER_H

#include "MessageFilter.h"
#include "gloox/client.h"
#include "JIDMap.h"

class ForeignMessageFilter : public MessageFilter
{
public:
    ForeignMessageFilter(JIDMap* usermap, Client *client);

    void handleMessage(const Message& stanza, bool room, bool priv, bool *handled);

private:
    JIDMap* _usermap;
    Client *_client;
};

#endif
