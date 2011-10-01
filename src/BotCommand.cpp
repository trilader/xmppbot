#include "BotCommand.h"

static BotCommand* tryFromString(std::string cmd, bool *success)
{
    std::vector<std::string> parts;
    boost::split(parts, cmd, boost::is_any_of(" "));

    if(parts.size() < 1)
    {
        success = false;
        return nullptr;
    }


}
