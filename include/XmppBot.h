#ifndef XMPPBOT_H
#define XMPPBOT_H

#include <iostream>
#include "gloox/client.h"
#include "gloox/message.h"
#include "gloox/presence.h"
#include "gloox/rosteritem.h"
#include "gloox/messagehandler.h"
#include "gloox/connectionlistener.h"
#include "gloox/rosterlistener.h"
#include "gloox/rostermanager.h"
#include "gloox/mucroom.h"
#include "gloox/mucroomhandler.h"
#include "gloox/adhoc.h"
#include "gloox/adhoccommandprovider.h"
#include "BotCommandManager.h"
#include "TestBotCommand.h"

using namespace gloox;

class XmppBot: public MessageHandler, ConnectionListener, RosterListener, MUCRoomHandler, AdhocCommandProvider
{
public:
    XmppBot();
    virtual ~XmppBot();
protected:
    // MessageListener
    void handleMessage(const Message& stanza, MessageSession* session = 0);

    // ConnectionListener
    void onConnect();
    void onDisconnect(ConnectionError e);
    bool onTLSConnect(const CertInfo &info);

    // RosterListener
    void handleItemAdded( const JID& jid );
    void handleItemSubscribed( const JID& jid );
    void handleItemRemoved( const JID& jid );
    void handleItemUpdated( const JID& jid );
    void handleItemUnsubscribed( const JID& jid );
    void handleRoster( const Roster& roster );
    void handleRosterPresence( const RosterItem& item, const std::string& resource, Presence::PresenceType presence, const std::string& msg );
    void handleSelfPresence( const RosterItem& item, const std::string& resource, Presence::PresenceType presence, const std::string& msg );
    bool handleSubscriptionRequest( const JID& jid, const std::string& msg );
    bool handleUnsubscriptionRequest( const JID& jid, const std::string& msg );
    void handleNonrosterPresence( const Presence& presence );
    void handleRosterError( const IQ& iq );

    // MUCRoomListener
    void handleMUCParticipantPresence( MUCRoom* room, const MUCRoomParticipant participant, const Presence& presence );
    void handleMUCMessage( MUCRoom* room, const Message& msg, bool priv );
    bool handleMUCRoomCreation( MUCRoom* room );
    void handleMUCSubject( MUCRoom* room, const std::string& nick, const std::string& subject );
    void handleMUCInviteDecline( MUCRoom* room, const JID& invitee, const std::string& reason );
    void handleMUCError( MUCRoom* room, StanzaError error );
    void handleMUCInfo( MUCRoom* room, int features, const std::string& name, const DataForm* infoForm );
    void handleMUCItems( MUCRoom* room, const Disco::ItemList& items );

    //AdhocCommandProvider
    void handleAdhocCommand( const JID& from, const Adhoc::Command& command, const std::string& sessionID );
    bool handleAdhocAccessRequest( const JID& from, const std::string& command );

private:
    Client* m_Client;
    RosterManager* m_RosterManager;
    MUCRoom* m_Room;
    Adhoc* m_Adhoc;
    BotCommandManager* m_CommandMgr;
};

#endif // XMPPBOT_H
