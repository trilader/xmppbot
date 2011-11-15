#ifndef LOGMESSAGEFILTER_H
#define LOGMESSAGEFILTER_H

#include "MessageFilter.h"
#include "LogHelper.h"

class LogMessageFilter : public MessageFilter
{
public:
    void handleMessage(const Message& stanza, bool room, bool priv, bool *handled);
};

#endif
