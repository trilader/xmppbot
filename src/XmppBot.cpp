#include "XmppBot.h"

#include "TestBotCommand.h"
#include "SubjectBotCommand.h"
#include "KickBotCommand.h"
#include "StateBotCommand.h"
#include "AliasBotCommand.h"
#include "HelpBotCommand.h"
#include "AdminBotCommand.h"
#include "ConfigurationBotCommand.h"

#include "HistoryMessageFilter.h"
#include "CommandMessageFilter.h"
#include "ForeignMessageFilter.h"
#include "LogMessageFilter.h"
#include "LinkMessageFilter.h"
#include "JIDMessageFilter.h"

XmppBot::XmppBot(Configuration *config)
{
    this->m_Config = config;
    this->init();
}

XmppBot::~XmppBot()
{
    if(m_Client!=NULL)
        delete m_Client;

    if(m_CommandMgr!=NULL)
        delete m_CommandMgr;

    if(m_UserNicknameMap!=NULL)
        delete m_UserNicknameMap;

    m_UserNicknameMap=NULL;
    m_CommandMgr=NULL;
    m_Client=NULL;

    LOG_DELETE(link)
    LOG_DELETE(chat)
    LOG_DELETE(command)
    LOG_DELETE(debug)
}

//init
void XmppBot::init()
{
    LOG(sys) << "Initializing...";

    LOG(sys) << "Init logs...";
    //init logs

    LOG_INIT(this->m_Config,debug)
    LOG_INIT(this->m_Config,command)
    LOG_INIT(this->m_Config,chat)
    LOG_INIT(this->m_Config,link)

    LOG(sys) << "Done.";

    LOG(sys) << "Init Xmpp...";

    this->initXmpp();

    LOG(sys) << "Done.";

    LOG(sys) << "Init commands...";

    this->initCommands();

    LOG(sys) << "Done.";

    LOG(sys) << "Init message filter...";

    this->initMessageFilter();

    LOG(sys) << "Done.";

    LOG(sys) << "Finalize...";

    msg_join= this->m_Config->getCustomItem("message.join");
    msg_leave= this->m_Config->getCustomItem("message.leave");
    msg_subscribe= this->m_Config->getCustomItem("message.subscribe");

    this->m_bePolite = this->m_Config->getCustomItem("polite") == "yes";

    LOG(sys) << "Initilization completed.";
}

void XmppBot::initXmpp()
{
    std::string username;
    std::string password;
    std::string server;
    std::string resource;
    std::string muc_name;
    std::string muc_room;
    std::string muc_server;

    this->m_Config->getXmppUser(&username, &password, &server, &resource);

    this->m_Config->getXmppMUC(&muc_name, &muc_room, &muc_server);

    m_UserNicknameMap = new JIDMap();

    JID nick;
    nick.setUsername(username);
    nick.setResource(resource);
    nick.setServer(server);

    LOG(debug)<< "Connecting to "<<server<<" as "<<username<<"/"<<resource;
    m_Client = new Client( nick, password);

    m_RosterManager = m_Client->rosterManager();
    m_RosterManager->registerRosterListener(this);

    JID muc_nick;
    muc_nick.setUsername(muc_room);
    muc_nick.setResource(muc_name);
    muc_nick.setServer(muc_server);

    LOG(debug)<< "Joining room "<<muc_room<<" on server "<<muc_server<<" as "<<muc_name;
    m_Room = new MUCRoom(m_Client, muc_nick, this, this);
    m_Client->registerMessageHandler(this);
    m_Client->registerConnectionListener(this);
}

void XmppBot::initCommands()
{
    this->m_CommandMgr = new BotCommandManager();
    //this->m_CommandMgr->registerCommand("test", new TestBotCommand());
    this->m_CommandMgr->registerCommand("kick", new KickBotCommand(this->m_Client,this->m_Room,this->m_Config));

    this->m_CommandMgr->registerCommand("setsbj", new SubjectBotCommand(this->m_Room, this->m_Config));

    this->m_CommandMgr->registerCommand("help", new HelpBotCommand(m_CommandMgr->getCommands()));

    this->m_StateCommand = new StateBotCommand(this->m_Room);
    this->m_CommandMgr->registerCommand("state", this->m_StateCommand);

    // register aliases for this->m_StateCommand
    this->m_CommandMgr->registerCommand("afk", new AliasBotCommand("afk ","","[<message>] - Go afk. An optional message cam be set",true,this->m_StateCommand));
    this->m_CommandMgr->registerCommand("re", new AliasBotCommand("re ","","Come back from being afk",true,this->m_StateCommand));

    this->m_CommandMgr->registerCommand("admin", new AdminBotCommand(this->m_Config, this));

    this->m_CommandMgr->registerCommand("config", new ConfigurationBotCommand(this->m_Config));
}

void XmppBot::initMessageFilter()
{
    std::string link_protos;

    if(this->m_Config->isCustomFilterItemSet("link", "protocols"))
        link_protos = this->m_Config->getCustomFilterItem("link", "protocols");
    else
        link_protos = "http,ftp";

    std::set<std::string> *jidexceptions = new std::set<std::string>();
    if(this->m_Config->isCustomFilterItemSet("foreign", "authalways"))
    {
        std::vector<std::string> out;
        std::string jidexceptions_string = this->m_Config->getCustomFilterItem("foreign", "authalways");
        boost::algorithm::split(out, jidexceptions_string,
                                boost::algorithm::is_any_of(","));

        for(unsigned int i=0; i<out.size(); i++)
        {
            jidexceptions->insert(out[i]);
            LOG(debug) << "Added " + out[i] + " to exception list";
        }
    }

    this->m_MessageFilter = new std::list<MessageFilter*>();

    //order is important!
    this->m_MessageFilter->push_back(new HistoryMessageFilter());
    this->m_MessageFilter->push_back(new ForeignMessageFilter(this->m_UserNicknameMap, jidexceptions, this->m_Client));

    if(jidexceptions->find(this->m_Client->jid().bare()) == jidexceptions->end())
        this->m_MessageFilter->push_back(new JIDMessageFilter(this->m_Client->jid(), true, true, true));
    else
        LOG(debug) << "warning: the bot is able to send himself commands ( 'authalways' configuration directive ), it is up to you to ensure correct priority settings!";

    this->m_MessageFilter->push_back(new CommandMessageFilter(this->m_CommandMgr,this->m_Client,this->m_UserNicknameMap));
    this->m_MessageFilter->push_back(new LinkMessageFilter(link_protos));
    this->m_MessageFilter->push_back(new LogMessageFilter());
}

XmppBot::ExitState XmppBot::run()
{
    LOG(sys) << "Started.";

    this->m_ExitState = XmppBot::QUIT;

    LOG(debug) << "Connecting...";
    if(m_Client->connect(false))
	{
		gloox::ConnectionError error = gloox::ConnectionError::ConnNoError;
		while(error == gloox::ConnectionError::ConnNoError)
			error = m_Client->recv( 1000*100 ); //100 mSec

		LOG(sys)<<"Connection error: "<<error;
	}
	//m_Client->disconnect();

	LOG(debug) << "Disconnected.";
    LOG(sys) << "Stoping...";

    return this->m_ExitState;
}

void XmppBot::quit(XmppBot::ExitState state)
{
	this->m_ExitState = state;

	this->m_Room->leave();
	this->m_Client->disconnect();
}

void XmppBot::handleMessage( const Message& stanza, MessageSession* session)
{
    this->handleMessage(stanza, false, true);
}

void XmppBot::onConnect()
{
    LOG(debug) << "Connected";

    m_Client->setPresence(Presence::Available, 0);

    m_Room->join();

    if(this->m_bePolite && msg_join!="")
        this->m_Room->send(msg_join);
}

void XmppBot::onDisconnect(ConnectionError e)
{
    if(this->m_bePolite && msg_leave!="")
        this->m_Room->send(msg_leave);
}

bool XmppBot::onTLSConnect(const CertInfo& info)
{
//    std::cout << "Certificate accepted!" << std::endl;
    return true;
}

void XmppBot::handleItemAdded( const JID& jid )
{
//    std::cout << "Added: " << jid.full() << std::endl;
}

void XmppBot::handleItemSubscribed( const JID& jid )
{
//    std::cout << "Subscribed to: " << jid.full() << std::endl;
}

void XmppBot::handleItemRemoved( const JID& jid )
{
//    std::cout << "Removed: " << jid.full() << std::endl;
}

void XmppBot::handleItemUpdated( const JID& jid )
{
//    std::cout << "Updated: " << jid.full() << std::endl;
}

void XmppBot::handleItemUnsubscribed( const JID& jid )
{
//    std::cout << "Unsubscribed from: " << jid.full() << std::endl;
}

void XmppBot::handleRoster( const Roster& roster )
{
/*    std::cout << "My roster:" << std::endl;
    for(Roster::const_iterator it=roster.begin(); it!=roster.end(); it++)
        std::cout << ((RosterItem*)(*it).second)->jid() << std::endl;
    std::cout << "End of roster." << std::endl;*/
}

void XmppBot::handleRosterPresence( const RosterItem& item, const std::string& resource, Presence::PresenceType presence, const std::string& msg )
{
//    std::cout << "Roster presence update: " << item.jid() << ": " << msg << std::endl;
}

void XmppBot::handleSelfPresence( const RosterItem& item, const std::string& resource, Presence::PresenceType presence, const std::string& msg )
{

}

bool XmppBot::handleSubscriptionRequest( const JID& jid, const std::string& msg )
{
//    std::cout << "Got a subscription request from: " << jid.full() << std::endl;
    m_RosterManager->ackSubscriptionRequest(jid, true);
    m_RosterManager->subscribe(jid,m_Client->jid().username(),StringList(),msg_subscribe);
    m_RosterManager->synchronize();
    return true;
}

bool XmppBot::handleUnsubscriptionRequest( const JID& jid, const std::string& msg )
{
//    std::cout << "Got an unsubscribe request from: " << jid.full() << std::endl;
    m_RosterManager->unsubscribe(jid);
    m_RosterManager->synchronize();
    return true;
}

void XmppBot::handleNonrosterPresence( const Presence& presence )
{
//    std::cout << " Got a non-roster presence: " << presence.from().full() << ": " << presence.status() << std::endl;
}

void XmppBot::handleRosterError( const IQ& iq )
{
//    std::cerr << "Roster error: " << iq.error() << std::endl;
}

void XmppBot::handleMUCParticipantPresence( MUCRoom* room, const MUCRoomParticipant participant, const Presence& presence )
{
	if(participant.jid == NULL)
	{
		LOG(debug) << "Note: invalid participant object received from server... ( no JID )!\nnick is " + participant.nick->full() + "!";
		return;
	}

    if( presence.presence() == Presence::Unavailable )
    {
        this->m_StateCommand->removeUserState(*(participant.nick));
        m_UserNicknameMap->erase(*(participant.jid));

        LOG(debug) << "Note: participant with JID " + participant.jid->full() + " left the room!";
    }
    else
    {
        (*m_UserNicknameMap)[*(participant.jid)]=*participant.nick;

        LOG(debug) << "Note: participant with JID " + participant.jid->full() + " joined or updated his status!";
    }
}

void XmppBot::handleMUCMessage( MUCRoom* room, const Message& stanza, bool priv )
{
    this->handleMessage(stanza, true, priv);
}

void XmppBot::handleMessage(const Message& stanza, bool room, bool priv)
{
    MessageInfo info(stanza, room, priv);

    std::list<MessageFilter*>::const_iterator it = this->m_MessageFilter->begin();
    for(; it!=this->m_MessageFilter->end() && !(info.isHandled()) ; it++)
            (*it)->handleMessage(&info);
}

bool XmppBot::handleMUCRoomCreation( MUCRoom* room )
{
    return true;
}

void XmppBot::handleMUCSubject( MUCRoom* room, const std::string& nick, const std::string& subject )
{

}

void XmppBot::handleMUCInviteDecline( MUCRoom* room, const JID& invitee, const std::string& reason )
{

}

void XmppBot::handleMUCError( MUCRoom* room, StanzaError error )
{
	LOG(debug) << "MUC error occured in room \"" + room->name() + "\" ( code " + boost::lexical_cast<std::string>(error) + " )";
}

void XmppBot::handleMUCInfo( MUCRoom* room, int features, const std::string& name, const DataForm* infoForm )
{

}

void XmppBot::handleMUCItems( MUCRoom* room, const Disco::ItemList& items )
{
/*
    std::cout << "Users in " << room->name() << ":" << std::endl;

    for(Disco::ItemList::const_iterator it = items.begin(); it!=items.end(); it++)
    {
        std::cout << ((Disco::Item*)(*it))->jid().resource() << std::endl;
    }
*/
}

void XmppBot::handleMUCConfigList( MUCRoom* room, const MUCListItemList& items, MUCOperation operation )
{

}

void XmppBot::handleMUCConfigForm( MUCRoom* room, const DataForm& form )
{

}

void XmppBot::handleMUCConfigResult( MUCRoom* room, bool success, MUCOperation operation )
{
//    std::cout << ""<<operation << " in " << room->name() << " " << (success?"succeeded":"failed") << std::endl;
}

void XmppBot::handleMUCRequest( MUCRoom* room, const DataForm& form )
{

}
