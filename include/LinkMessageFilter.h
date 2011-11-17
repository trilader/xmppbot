#ifndef LINKMESSAGEFILTER_H
#define LINKMESSAGEFILTER_H

#include "MessageFilter.h"
#include "StringFormat.h"
#include "LogHelper.h"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

#include "gloox/client.h"

class LinkMessageFilter : public MessageFilter
{
public:
    LinkMessageFilter(const std::string& protocols);

    void handleMessage(const Message& stanza, bool room, bool priv, bool *handled);
    void setLogFormat(StringFormat *format);

private:
    std::string _protocols;
    StringFormat *_format;
    boost::regex* _re;
};

#endif
