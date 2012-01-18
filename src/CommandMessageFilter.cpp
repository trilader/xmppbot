#include "CommandMessageFilter.h"

CommandMessageFilter::CommandMessageFilter(BotCommandManager *mgr, Client *client, JIDMap* usermap)
{
    this->_client = client;
    this->_manager = mgr;
    this->_usermap = usermap;

    this->setLogFormat(new StringFormat("Invoked command \"%1\" from \"%2\". Response message: \"%3\"."),
                        new StringFormat("Failed to invoke command \"%1\" from \"%2\". Error message: \"%3\"."));
}

void CommandMessageFilter::setLogFormat(StringFormat *success, StringFormat *fail)
{
    this->_failformat = fail;
    this->_successformat = success;
}

void CommandMessageFilter::handleMessage(MessageInfo *info)
{
    std::string msg = info->getMessage().body();

    if(msg.size() <= 0)
        return;

    if(info->isRoom() && !info->isPrivate() && msg.at(0)!='!')
        return;

    if(msg.at(0)=='!')
        msg = msg.substr(1);

    std::string response;

    JID from = ((info->isRoom())    ? info->getMessage().from()
                                    : (*(this->_usermap))[info->getMessage().from()]);

    bool success = this->_manager->tryInvoke(info, &response);

    StringFormat *logformat = (success) ? this->_successformat : this->_failformat;
    logformat->assign("1", msg);
    logformat->assign("2", from.full());
    logformat->assign("3", response);

    LOG(command) << logformat->produce();

    if(!success || info->isPrivate() || !info->isRoom())
    {
        Message m(Message::Chat, info->getMessage().from(),response);
        this->_client->send(m);
    }

    info->markHandled();
}
