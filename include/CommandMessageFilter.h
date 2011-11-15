#ifndef COMMANDMESSAGEFILTER_H
#define COMMANDMESSAGEFILTER_H

#include "MessageFilter.h"
#include "StringFormat.h"
#include "BotCommandManager.h"
#include "LogHelper.h"
#include "JIDMap.h"

#include "gloox/client.h"

class CommandMessageFilter : public MessageFilter
{
public:
    CommandMessageFilter(BotCommandManager *mgr, Client *client, JIDMap* usermap);

    void handleMessage(const Message& stanza, bool room, bool priv, bool *handled);
    void setLogFormat(StringFormat *success, StringFormat *fail);

private:
    StringFormat *_successformat;
    StringFormat *_failformat;

    BotCommandManager *_manager;
    Client *_client;

    JIDMap *_usermap;
};

#endif
