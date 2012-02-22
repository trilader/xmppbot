#include "HistoryMessageFilter.h"

void HistoryMessageFilter::handleMessage(MessageInfo *info)
{
    if(info->getWhen() != NULL)
        info->markHandled();
}
