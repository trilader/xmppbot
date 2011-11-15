#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "gloox/message.h"

using namespace gloox;

class MessageFilter
{
public:
    virtual void handleMessage(const Message& stanza, bool room, bool priv, bool *handled) = 0;
};

#endif
