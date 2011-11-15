#include "LogMessageFilter.h"

void LogMessageFilter::handleMessage(const Message& stanza, bool room, bool priv, bool *handled)
{
    if(!room)
        return;

    LOG(chat) << stanza.from().resource() + ": " + stanza.body();
}
