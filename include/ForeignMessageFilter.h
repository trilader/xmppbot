#ifndef FOREIGNMESSAGEHANDLER_H
#define FOREIGNMESSAGEHANDLER_H

#include "MessageFilter.h"
#include "gloox/client.h"
#include <set>
#include "JIDMap.h"
#include "LogHelper.h"

class ForeignMessageFilter : public MessageFilter
{
public:
    ForeignMessageFilter(JIDMap* usermap, std::set<std::string> *authusers, Client *client);

    void handleMessage(const Message& stanza, bool room, bool priv, bool *handled);

private:
    JIDMap* _usermap;
    std::set<std::string> *_authUsers;
    Client *_client;
};

#endif
