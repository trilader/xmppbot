#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "MessageInfo.h"
#include "ConfigurationBased.h"

using namespace gloox;

class MessageFilter : public ConfigurationBased
{
public:
    MessageFilter(Configuration *config = 0);

    virtual void handleMessage(MessageInfo *info) = 0;

protected:
    virtual std::string getOption(const std::string& name, const std::string& option) const;
};

#endif
