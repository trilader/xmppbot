#include "HistoryMessageFilter.h"

void HistoryMessageFilter::handleMessage(MessageInfo *info)
{
    if(info->getMessage().when() != NULL)
        info->markHandled();
}
