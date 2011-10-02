#include "XmppBot.h"

XmppBot::XmppBot()
{
    std::cout << "Initializing..." << std::endl;

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
        ;

    std::ifstream ifs("bot.cfg");
    opt::store(opt::parse_config_file(ifs, desc), vm);
    opt::notify(vm);

    std::cout << "Loaded \"bot.cfg\". Parsing... ";

    std::string username;
    std::string password;
    std::string server;
    std::string resource;
    std::string muc_name;
    std::string muc_room;
    std::string muc_server;
    std::string admin_pw;

    std::string subject_event_name;
    std::string subject_event_date;

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
    if(vm.count("command.admin.password"))
        admin_pw = vm["command.admin.password"].as<std::string>();

    if(vm.count("command.subject.eventname") && vm.count("command.subject.eventdate"))
    {
        subject_event_name = vm["command.subject.eventname"].as<std::string>();
        subject_event_date = vm["command.subject.eventdate"].as<std::string>();
    }

    std::cout << "Done." << std::endl;

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

    this->m_CommandMgr = new BotCommandManager();
    this->m_CommandMgr->registerCommand("test", new TestBotCommand());
    this->m_CommandMgr->registerCommand("kick", new KickCommand(m_Client,m_Room,admin_pw));

    SubjectBotCommand *subjcmd = new SubjectBotCommand(this->m_Room, admin_pw);
    if(subject_event_name.length() > 0)
        subjcmd->setEvent(subject_event_name, subject_event_date);

    this->m_CommandMgr->registerCommand("setsbj", subjcmd);

    m_Client->connect();
}

XmppBot::~XmppBot()
{
    if(m_Client!=0)
    {
        if(m_Client->state()==StateConnected || m_Client->state()==StateConnecting)
        {
            m_Client->disconnect();
        }
    }
    delete m_Client;
    m_Client=0;
}

void XmppBot::handleMessage( const Message& stanza, MessageSession* session)
{
    std::cout << "Message from: "<< stanza.from().username() << ": " << stanza.body() << std::endl;

    std::string jid = stanza.from().bareJID().full();
    std::string msg = stanza.body();

    /*bool success = false;
    this->m_CommandMgr->tryInvokeFromString(msg, stanza.from(),&success);

    if(!success)
        std::cout <<  "invoke command failed" << std::endl;
*/
    Message m(Message::Chat, stanza.from(), "Hi, "+stanza.from().username()+" you wrote: \""+stanza.body()+"\".");
    m_Client->send(m);
}

void XmppBot::onConnect()
{
    std::cout << "Connected" << std::endl;
    m_Client->setPresence(Presence::Available, 0);

    m_Room->join();
    m_Room->getRoomItems();
    m_Room->requestList(GetRoomItems);
}

void XmppBot::onDisconnect(ConnectionError e)
{

}

bool XmppBot::onTLSConnect(const CertInfo& info)
{
    std::cout << "Certificate accepted!" << std::endl;
    return true;
}

void XmppBot::handleItemAdded( const JID& jid )
{
    std::cout << "Added: " << jid.full() << std::endl;
}

void XmppBot::handleItemSubscribed( const JID& jid )
{
    std::cout << "Subscribed to: " << jid.full() << std::endl;
}

void XmppBot::handleItemRemoved( const JID& jid )
{
    std::cout << "Removed: " << jid.full() << std::endl;
}

void XmppBot::handleItemUpdated( const JID& jid )
{
    std::cout << "Updated: " << jid.full() << std::endl;
}

void XmppBot::handleItemUnsubscribed( const JID& jid )
{
    std::cout << "Unsubscribed from: " << jid.full() << std::endl;
}

void XmppBot::handleRoster( const Roster& roster )
{
    std::cout << "My roster:" << std::endl;
    for(Roster::const_iterator it=roster.begin(); it!=roster.end(); it++)
        std::cout << ((RosterItem*)(*it).second)->jid() << std::endl;
    std::cout << "End of roster." << std::endl;
}

void XmppBot::handleRosterPresence( const RosterItem& item, const std::string& resource, Presence::PresenceType presence, const std::string& msg )
{
    std::cout << "Roster presence update: " << item.jid() << ": " << msg << std::endl;
}

void XmppBot::handleSelfPresence( const RosterItem& item, const std::string& resource, Presence::PresenceType presence, const std::string& msg )
{

}

bool XmppBot::handleSubscriptionRequest( const JID& jid, const std::string& msg )
{
    std::cout << "Got a subscription request from: " << jid.full() << std::endl;
    m_RosterManager->ackSubscriptionRequest(jid, true);
    m_RosterManager->subscribe(jid,"SVN-Bot",StringList(),"Hi I am the SVN-Bot from shin-project.org");
    m_RosterManager->synchronize();
    return true;
}

bool XmppBot::handleUnsubscriptionRequest( const JID& jid, const std::string& msg )
{
    std::cout << "Got an unsubscribe request from: " << jid.full() << std::endl;
    m_RosterManager->unsubscribe(jid);
    m_RosterManager->synchronize();
    return true;
}

void XmppBot::handleNonrosterPresence( const Presence& presence )
{
    std::cout << " Got a non-roster presence: " << presence.from().full() << ": " << presence.status() << std::endl;
}

void XmppBot::handleRosterError( const IQ& iq )
{
    std::cerr << "Roster error: " << iq.error() << std::endl;
}

void XmppBot::handleMUCParticipantPresence( MUCRoom* room, const MUCRoomParticipant participant, const Presence& presence )
{
    std::cout << "Room presence: "<<participant.nick->full()<<" new state: "<<presence.status()<<std::endl;
}

void XmppBot::handleMUCMessage( MUCRoom* room, const Message& stanza, bool priv )
{
    std::string msg = stanza.body();
    bool handle = priv;

    if(msg.size() > 0 && msg.at(0) == '!')
    {
        msg = msg.substr(1);
        handle = true;
    }

    if(!handle)
        return;

    std::string response;

    bool success = this->m_CommandMgr->tryInvokeFromString(msg, stanza.from(),&response);

    std::string state;
    if(success)
        state = "[ OK ]";
    else
        state = "[ fail ]";

    std::cout << state << " " << response << std::endl;
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
    std::cout <<"Error in " << room->name() << ": "<<error << std::endl;
}

void XmppBot::handleMUCInfo( MUCRoom* room, int features, const std::string& name, const DataForm* infoForm )
{

}

void XmppBot::handleMUCItems( MUCRoom* room, const Disco::ItemList& items )
{
    std::cout << "Users in " << room->name() << ":" << std::endl;

    for(Disco::ItemList::const_iterator it = items.begin(); it!=items.end(); it++)
    {
        std::cout << ((Disco::Item*)(*it))->jid().resource() << std::endl;
    }

}

void XmppBot::handleMUCConfigList( MUCRoom* room, const MUCListItemList& items, MUCOperation operation )
{

}

void XmppBot::handleMUCConfigForm( MUCRoom* room, const DataForm& form )
{

}

void XmppBot::handleMUCConfigResult( MUCRoom* room, bool success, MUCOperation operation )
{
    std::cout << ""<<operation << " in " << room->name() << " " << (success?"succeeded":"failed") << std::endl;
}

void XmppBot::handleMUCRequest( MUCRoom* room, const DataForm& form )
{

}
