#include "MessageFilter.h"

MessageFilter::MessageFilter(Configuration *config) : ConfigurationBased(config){}

std::string MessageFilter::getOption(const std::string& name, const std::string& option) const
{
    if(0 == this->getConfig())
        return "";

    return this->getConfig()->getCustomFilterItem(name, option);
}
