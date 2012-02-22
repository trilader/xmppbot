#ifndef MESSAGEINFO_H
#define MESSAGEINFO_H

#include "gloox/message.h"

class MessageInfo
{
    public:
        MessageInfo(const gloox::Message& msg, bool room, bool priv);

        bool isPrivate();
        bool isRoom();

        bool isHandled();
        void markHandled();

        std::string getBody();
        void setBody(const std::string& body);

        gloox::JID getFrom();
        void setFrom(const gloox::JID& jid);

        const gloox::DelayedDelivery* getWhen();

    private:
        std::string _body;
        gloox::JID _from;
        const gloox::DelayedDelivery *_when;
        bool _room;
        bool _private;
        bool _handled;
};

#endif
