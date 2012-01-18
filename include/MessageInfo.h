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

        const gloox::Message& getMessage();

    private:
        const gloox::Message& _message;
        bool _room;
        bool _private;
        bool _handled;
};

#endif
