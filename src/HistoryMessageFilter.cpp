#include "HistoryMessageFilter.h"

void HistoryMessageFilter::handleMessage(const Message& stanza, bool room, bool priv, bool *handled)
{
    *handled = (stanza.when()!=NULL);
}
