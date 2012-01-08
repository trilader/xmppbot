#include "Program.h"

bpo::variables_map Program::_vm;
Configuration *Program::_config;

int Program::main(int argc, char **argv)
{
    Program::parseOptions(argc, argv);

    if(Program::_vm.count("help"))
        return XmppBot::ExitState::QUIT;

    if(Program::_vm.count("silent"))
    {
        LOG_ADD(sys,new VoidLog());
    }
    else
    {
        LOG_ADD(sys,new ConsoleLog(new StringFormat("system: %_")));
    }

    if(!Program::loadConfiguration())
        return XmppBot::ExitState::CONFIG_NOT_FOUND;

    return Program::runBot();
}

void Program::parseOptions(int argc, char **argv)
{
    bpo::options_description desc("Options");
    desc.add_options()
        ("config,c", bpo::value<std::string>()->default_value("bot.cfg"), "the configuration file")
        ("configmode,m", bpo::value<std::string>()->default_value("writable"), "the configuration mode")
        ("edit,e", bpo::value<std::string>(), "configuration option to edit")
        ("value,v", bpo::value<std::string>(), "new value of the configuration option to edit")
        ("get", bpo::value<std::string>(), "configuration option to get and print out")
        ("silent", "start bot in silent mode ( no 'system' output )")
        ("help,h", "prints help")
    ;

    bpo::positional_options_description pdesc;
    pdesc.add("config", -1);

    bpo::store(bpo::command_line_parser(argc, argv).options(desc).positional(pdesc).run(), Program::_vm);
    bpo::notify(Program::_vm);

    if(Program::_vm.count("help"))
        std::cout << desc << std::endl;
}

bool Program::loadConfiguration()
{
    std::string cfgfile = "";
    if(Program::_vm.count("config"))
        cfgfile = Program::_vm["config"].as<std::string>();

    boost::trim(cfgfile);

    std::string cfgmode = "writable";
    if(Program::_vm.count("configmode"))
        cfgmode = Program::_vm["configmode"].as<std::string>();

    boost::algorithm::trim(cfgmode);

    bool readonly = false;
    if(cfgmode == "readonly")
        readonly = true;
    else if(cfgmode == "writable")
        readonly = false;
    else
    {
        LOG(sys) << "error: unknown configmode specified!";
        return false;
    }

    if(boost::algorithm::iends_with(cfgfile, ".cfg"))
    {
        if(!readonly)
            LOG(sys) << "warning: unable to load configuration with specified configmode 'writable'! Loading configuration 'readonly' instead...";

        Program::_config = new FileConfiguration(cfgfile);
    }
    else if(boost::algorithm::iends_with(cfgfile, ".db"))
    {
        Program::_config = new SQLiteConfiguration(cfgfile, !readonly);
    }
    else
    {
        LOG(sys) << "error: unknown config file-extension!";
        return false;
    }

    return true;
}

int Program::runBot()
{
	XmppBot::ExitState state = XmppBot::QUIT;

	do
	{
		XmppBot* b = new XmppBot(Program::_config);
		state = b->run();

		delete b; //call dtor

	} while(state < XmppBot::QUIT);

    return state;
}
