#ifndef HISTORYMESSAGEFILTER_H
#define HISTORYMESSAGEFILTER_H

#include "gloox/message.h"
#include "MessageFilter.h"

using namespace gloox;

class HistoryMessageFilter: public MessageFilter
{
public:
    void handleMessage(MessageInfo *info);
};

#endif
