#include "ConfigurationBased.h"

ConfigurationBased::ConfigurationBased(Configuration *config)
{
    this->_config = config;
}

ConfigurationBased::~ConfigurationBased()
{}

Configuration* ConfigurationBased::getConfig() const
{
    return this->_config;
}
