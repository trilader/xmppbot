#include "LogMessageFilter.h"

void LogMessageFilter::handleMessage(MessageInfo *info)
{
    if(!info->isRoom())
        return;

    LOG(chat) << info->getMessage().from().resource() + ": " + info->getMessage().body();
}
