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

    if(Program::_vm.count("sqldebug"))
    {
        LOG_ADD(sql,new ConsoleLog(new StringFormat("sql: %_")));
    }
    else
    {
        LOG_ADD(sql,new VoidLog());
    }

    if(!Program::loadConfiguration())
        return XmppBot::ExitState::CONFIG_NOT_FOUND;

    if(Program::_vm.count("edit") || Program::_vm.count("get"))
        return Program::editConfiguration();

    return Program::runBot();
}

void Program::parseOptions(int argc, char **argv)
{
    bpo::options_description desc("Options");
    desc.add_options()
        ("config,c", bpo::value<std::string>()->default_value("config.db"), "the configuration file")
        ("configmode,m", bpo::value<std::string>()->default_value("writable"), "the configuration mode")
        ("index,i", bpo::value<unsigned int>()->default_value(0), "the configuration index")
        ("edit,e", bpo::value<std::string>(), "configuration option to edit")
        ("value,v", bpo::value<std::string>(), "new value of the configuration option to edit")
        ("get,g", bpo::value<std::string>(), "configuration option to get and print out")
        ("silent", "start bot in silent mode ( no 'system' output )")
        ("help,h", "prints help")
    ;

    bpo::options_description hidden_desc("Hidden Options");
    hidden_desc.add_options()
        ("sqldebug", "turns on sql console log for debugging.")
    ;

    bpo::options_description all_desc("All");
    all_desc.add(desc).add(hidden_desc);

    bpo::positional_options_description pdesc;
    pdesc.add("config", -1);

    bpo::store(bpo::command_line_parser(argc, argv).options(all_desc).positional(pdesc).run(), Program::_vm);
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
    #if defined DB_SUPPORT && defined DB_SQLITE_SUPPORT
    else if(boost::algorithm::iends_with(cfgfile, ".db"))
    {
        Program::_config = new SQLiteConfiguration(cfgfile, !readonly);
    }
    #endif
    else
    {
        LOG(sys) << "error: unknown config file-extension!";
        return false;
    }

    unsigned int index = Program::_vm.count("index") ? Program::_vm["index"].as<unsigned int>() : 0;
    if(!Program::_config->setConfigurationIndex(index))
    {
        LOG(sys) << "warning: unable to set configuration index....";
    }

    return true;
}

int Program::editConfiguration()
{
    if(Program::_vm.count("get"))
    {
        std::string optionstr = Program::_vm["get"].as<std::string>();
        boost::algorithm::trim(optionstr);

        std::string value;
        if(Program::_config->getFromSeperatedString(optionstr, &value))
            LOG(sys) << "Value of configuration option '" + optionstr + "' is '" + value + "'.";
        else
            LOG(sys) << "Failed to get configuration option '" + optionstr + "'.";

        return XmppBot::ExitState::QUIT;
    }

    //edit
    if(Program::_vm.count("edit"))
    {
        if(!Program::_vm.count("value"))
        {
            LOG(sys) << "No value specified. Abort.";
            return XmppBot::ExitState::QUIT;
        }

        if(!Program::_config->isWritable())
        {
            LOG(sys) << "Configuration is not writable. Abort.";
            return XmppBot::ExitState::QUIT;
        }

        std::string optionstr = Program::_vm["edit"].as<std::string>(),
                    value = Program::_vm["value"].as<std::string>();

        boost::algorithm::trim(optionstr);
        boost::algorithm::trim(value);

        if(Program::_config->setFromSeperatedString(optionstr, value))
            LOG(sys) << "Set configuration option successfully!";
        else
            LOG(sys) << "Failed to set configuration option!";

        return XmppBot::ExitState::QUIT;
    }

    LOG(sys) << "No valid edit option specified...";
    return XmppBot::ExitState::QUIT;
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
