#include "XmppBot.h"

#include "TestBotCommand.h"
#include "SubjectBotCommand.h"
#include "KickBotCommand.h"
#include "StateBotCommand.h"
#include "AliasBotCommand.h"
#include "HelpBotCommand.h"
#include "AdminBotCommand.h"

#include "HistoryMessageFilter.h"
#include "CommandMessageFilter.h"
#include "ForeignMessageFilter.h"
#include "LogMessageFilter.h"
#include "LinkMessageFilter.h"

XmppBot::XmppBot(std::string configfile)
{
    this->m_ConfigFile = configfile;

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
}

//init
void XmppBot::init()
{
    LOG_ADD(sys,new ConsoleLog(new StringFormat("system: %_")))

    LOG(sys) << "Initializing...";

    this->initConfig();

    LOG(sys) << "Loaded \"" + this->m_ConfigFile + "\".";

    LOG(sys) << "Init logs...";
    //init logs

    LOG_INIT(vm,debug)
    LOG_INIT(vm,command)
    LOG_INIT(vm,chat)
    LOG_INIT(vm,link)

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

    std::string polite = "no";
    msg_join="";
    msg_leave="";
    msg_subscribe="";

    if(vm.count("bot.polite"))
        polite = vm["bot.polite"].as<std::string>();
    if(vm.count("bot.message.join"))
        msg_join = vm["bot.message.join"].as<std::string>();
    if(vm.count("bot.message.leave"))
        msg_leave = vm["bot.message.leave"].as<std::string>();
    if(vm.count("bot.message.subscribe"))
        msg_subscribe = vm["bot.message.subscribe"].as<std::string>();

    this->m_bePolite = polite == "yes";

    LOG(sys) << "Initilization completed.";
}

void XmppBot::initConfig()
{
    opt::options_description desc("Options");
    desc.add_options()
        ("server.user","User name")
        ("server.password","Password")
        ("server.address","The server to connect to")
        ("server.resource","The resource to connect as")
        ("muc.name","The name to use in the MUC room")
        ("muc.room","The MUC room to connect to")
        ("muc.server","The MUC server to connect to")
        ("command.admin.password","Password to use admin function")
        ("command.subject.eventname","Name of an important event which should be mentioned in the room subject")
        ("command.subject.eventdate","Date of an important event ( depends on command.subject.eventname ); format: year-month-day eg: 2002-1-25")
        ("command.subject.format","Format string for the subject command")
        ("bot.polite","if enabled the bot says \"hi\" and \"goodbye\"")
        ("bot.message.join","Message on joining in polite-mode")
        ("bot.message.leave","Message on leaving in polite-mode")
        ("bot.message.subscribe","Message the bot sends clients who want to subscribe to the bot")
        ("filter.link.protocols","Which URI protocols the bot should look for")
        ("filter.foreign.authalways","")
        LOG_CONFIG(debug)
        LOG_CONFIG(command)
        LOG_CONFIG(chat)
        LOG_CONFIG(link)
        ;

    std::ifstream ifs(this->m_ConfigFile);
    opt::store(opt::parse_config_file(ifs, desc), vm);
    opt::notify(vm);
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

    if(vm.count("server.user"))
        username = vm["server.user"].as<std::string>();
    if(vm.count("server.password"))
        password = vm["server.password"].as<std::string>();
    if(vm.count("server.address"))
        server = vm["server.address"].as<std::string>();
    if(vm.count("server.resource"))
        resource = vm["server.resource"].as<std::string>();
    if(vm.count("muc.name"))
        muc_name = vm["muc.name"].as<std::string>();
    if(vm.count("muc.room"))
        muc_room = vm["muc.room"].as<std::string>();
    if(vm.count("muc.server"))
        muc_server = vm["muc.server"].as<std::string>();

    m_UserNicknameMap = new JIDMap();

    JID nick;
    nick.setUsername(username);
    nick.setResource(resource);
    nick.setServer(server);

    m_Client = new Client( nick, password);

    m_RosterManager = m_Client->rosterManager();
    m_RosterManager->registerRosterListener(this);

    JID muc_nick;
    muc_nick.setUsername(muc_room);
    muc_nick.setResource(muc_name);
    muc_nick.setServer(muc_server);

    m_Room = new MUCRoom(m_Client, muc_nick, this, this);
    m_Client->registerMessageHandler(this);
    m_Client->registerConnectionListener(this);
}

void XmppBot::initCommands()
{
    std::string admin_pw;

    std::string subject_event_name;
    std::string subject_event_date;
    std::string subject_format = "%3";

    if(vm.count("command.admin.password"))
        admin_pw = vm["command.admin.password"].as<std::string>();

    if(vm.count("command.subject.eventname") && vm.count("command.subject.eventdate"))
    {
        subject_event_name = vm["command.subject.eventname"].as<std::string>();
        subject_event_date = vm["command.subject.eventdate"].as<std::string>();
    }

    if(vm.count("command.subject.format"))
        subject_format = vm["command.subject.format"].as<std::string>();

    this->m_CommandMgr = new BotCommandManager();
    //this->m_CommandMgr->registerCommand("test", new TestBotCommand());
    this->m_CommandMgr->registerCommand("kick", new KickBotCommand(this->m_Client,this->m_Room,admin_pw));

    SubjectBotCommand *subjcmd = new SubjectBotCommand(this->m_Room, admin_pw, new StringFormat(subject_format));
    if(subject_event_name.length() > 0)
        subjcmd->setEvent(subject_event_name, subject_event_date);

    this->m_CommandMgr->registerCommand("setsbj", subjcmd);
    this->m_CommandMgr->registerCommand("help", new HelpBotCommand(m_CommandMgr->getCommands()));

    this->m_StateCommand = new StateBotCommand(this->m_Room);
    this->m_CommandMgr->registerCommand("state", this->m_StateCommand);

    // register aliases for this->m_StateCommand
    this->m_CommandMgr->registerCommand("afk", new AliasBotCommand("afk ","","[<message>] - Go afk. An optional message cam be set",true,this->m_StateCommand));
    this->m_CommandMgr->registerCommand("re", new AliasBotCommand("re ","","Come back from being afk",true,this->m_StateCommand));

    this->m_CommandMgr->registerCommand("admin", new AdminBotCommand(admin_pw, this));
}

void XmppBot::initMessageFilter()
{
    std::string link_protos;

    if(vm.count("filter.link.protocols"))
        link_protos = vm["filter.link.protocols"].as<std::string>();
    else
        link_protos = "http,ftp";

    std::set<gloox::JID> *jidexceptions = new std::set<gloox::JID>();
    if(vm.count("filter.foreign.authalways"))
    {
        std::vector<std::string> out;
        boost::algorithm::split(out, vm["filter.foreign.authalways"].as<std::string>(),
                                boost::algorithm::is_any_of(","));

        for(unsigned int i=0; i<out.size(); i++)
        {
            gloox::JID jid(out[i]);
            jidexceptions->insert(jid);
            LOG(debug) << "Added " + jid.full() + " to exception list";
        }
    }

    this->m_MessageFilter = new std::list<MessageFilter*>();

    //order is important!
    this->m_MessageFilter->push_back(new HistoryMessageFilter());
    this->m_MessageFilter->push_back(new ForeignMessageFilter(this->m_UserNicknameMap, jidexceptions, this->m_Client));
    this->m_MessageFilter->push_back(new CommandMessageFilter(this->m_CommandMgr,this->m_Client,this->m_UserNicknameMap));
    this->m_MessageFilter->push_back(new LinkMessageFilter(link_protos));
    this->m_MessageFilter->push_back(new LogMessageFilter());
}

XmppBot::ExitState XmppBot::run()
{
    LOG(sys) << "Started.";

    this->m_ExitState = XmppBot::QUIT;

    LOG(debug) << "Connecting...";
    m_Client->connect(); //blocks
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
    bool handled = false;

    std::list<MessageFilter*>::const_iterator it = this->m_MessageFilter->begin();
    for(; it!=this->m_MessageFilter->end() && !handled ; it++)
            (*it)->handleMessage(stanza, room, priv, &handled);
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
