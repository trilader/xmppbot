#ifndef CONFIGURATIONBASED_H
#define CONFIGURATIONBASED_H

#include "Configuration.h"
#include <string>

class ConfigurationBased
{
    public:
        ConfigurationBased(Configuration *config);
        virtual ~ConfigurationBased();

    protected:
        virtual std::string getOption(const std::string& name, const std::string& option) const = 0;
        Configuration* getConfig() const;

    private:
        Configuration *_config;
};

#endif
