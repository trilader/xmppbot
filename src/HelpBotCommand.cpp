#include "HelpBotCommand.h"

HelpBotCommand::HelpBotCommand(const boost::unordered_map<std::string, BotCommand*>* commandList)
{
    m_CommandList = commandList;
}

bool HelpBotCommand::invoke(const JID& user, const bool priv, const std::string& args, std::string *response) const
{
    if(m_CommandList==NULL)
    {
        *response = "Internal error!";
        return false;
    }

    std::stringstream ss;
    ss << "Available commands:"<<std::endl;
    for(boost::unordered_map<std::string, BotCommand*>::const_iterator it=m_CommandList->begin(); it!=m_CommandList->end(); it++)
    {
        if(it->second->showHelp())
            ss<<it->first<<"\t"<<it->second->getHelp()<<std::endl;
    }

    *response = ss.str();
    return true;
}

std::string HelpBotCommand::getHelp() const
{
    return std::string("Get help and usage information");
}

bool HelpBotCommand::showHelp() const
{
    return false;
}
