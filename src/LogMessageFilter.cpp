#include "LogMessageFilter.h"

void LogMessageFilter::handleMessage(MessageInfo *info)
{
    if(!info->isRoom())
        return;

    LOG(chat) << info->getFrom().resource() + ": " + info->getBody();
}
