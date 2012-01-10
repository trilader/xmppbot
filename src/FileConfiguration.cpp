#include "FileConfiguration.h"

FileConfiguration::FileConfiguration(std::string file)
{
    opt::options_description desc("Options");
    desc.add_options()
        ((this->getUserOptionIdent()   + ".user").c_str(),"User name")
        ((this->getUserOptionIdent()   + ".password").c_str(),"Password")
        ((this->getUserOptionIdent()   + ".address").c_str(),"The server to connect to")
        ((this->getUserOptionIdent()   + ".resource").c_str(),"The resource to connect as")
        ((this->getMUCOptionIdent()    + ".name").c_str(),"The name to use in the MUC room")
        ((this->getMUCOptionIdent()    + ".room").c_str(),"The MUC room to connect to")
        ((this->getMUCOptionIdent()    + ".server").c_str(),"The MUC server to connect to")
        ((this->getCommandOptionIdent()    + ".admin.password").c_str(),"Password to use admin function")
        ((this->getCommandOptionIdent()    + ".subject.eventname").c_str(),"Name of an important event which should be mentioned in the room subject")
        ((this->getCommandOptionIdent()    + ".subject.eventdate").c_str(),"Date of an important event ( depends on command.subject.eventname ); format: year-month-day eg: 2002-1-25")
        ((this->getCommandOptionIdent()    + ".subject.format").c_str(),"Format string for the subject command")
        ((this->getCustomOptionIdent() + ".polite").c_str(),"if enabled the bot says \"hi\" and \"goodbye\"")
        ((this->getCustomOptionIdent() + ".message.join").c_str(),"Message on joining in polite-mode")
        ((this->getCustomOptionIdent() + ".message.leave").c_str(),"Message on leaving in polite-mode")
        ((this->getCustomOptionIdent() + ".message.subscribe").c_str(),"Message the bot sends clients who want to subscribe to the bot")
        ((this->getFilterOptionIdent() + ".link.protocols").c_str(),"Which URI protocols the bot should look for")
        ((this->getFilterOptionIdent() + ".foreign.authalways").c_str(),"")
        ;

    this->addLogOptions("debug", &desc);
    this->addLogOptions("command", &desc);
    this->addLogOptions("chat", &desc);
    this->addLogOptions("link", &desc);

    std::ifstream ifs(file);
    opt::store(opt::parse_config_file(ifs, desc), vm);
    opt::notify(vm);
}

void FileConfiguration::addLogOptions(const std::string& name, opt::options_description *desc)
{
    desc->add_options()
        ((this->getLogOptionIdent() + "." + name + ".enabled").c_str(), "(yes|no) Enable or disable the log") \
        ((this->getLogOptionIdent() + "." + name + ".type").c_str(), "(console|file) The type of the log") \
        ((this->getLogOptionIdent() + "." + name + ".entryformat").c_str(), "Custom format string") \
        ((this->getLogOptionIdent() + "." + name + ".fileformat").c_str(), "Custom filename format string ( only for type = file") \
        ((this->getLogOptionIdent() + "." + name + ".keepopen").c_str(), "");
}

bool FileConfiguration::isWritable()
{
    return false;
}

bool FileConfiguration::setConfigurationIndex(unsigned int index)
{
    return false;
}

unsigned int FileConfiguration::getConfigurationIndex()
{
    return 0;
}

bool FileConfiguration::isItemSet(const std::string& option)
{
    return vm.count(option);
}

std::string FileConfiguration::getItem(const std::string& option)
{
    if(this->isItemSet(option))
        return vm[option].as<std::string>();

    return "";
}

bool FileConfiguration::setLog(const std::string& name, const std::string& type, bool enabled, bool keepopen)
{
    return this->setLog(name, type, "", "", enabled, keepopen);
}

bool FileConfiguration::setLog(const std::string& name, const std::string& type, const std::string& fileformat, const std::string& entryformat, bool enabled = true, bool keepopen = false)
{
    return false;
}

bool FileConfiguration::getLog(const std::string& name, std::string *type, bool *enabled, bool *keepopen)
{
    std::string prefix = this->getLogOptionIdent() + "." + name;

    *type = this->getItem(prefix + ".type");
    *enabled = this->getItem(prefix + ".enabled") == "yes";
    *keepopen = this->getItem(prefix + ".keepopen") == "yes";

    return this->isItemSet(prefix + ".type") && this->isItemSet(prefix + ".enabled"); //note: keepopen is optional
}

bool FileConfiguration::getLog(const std::string& name, std::string *type, std::string *fileformat, std::string *entryformat, bool *enabled, bool *keepopen)
{
    std::string prefix = this->getLogOptionIdent() + "." + name;

    *fileformat = this->getItem(prefix + ".fileformat");
    *entryformat = this->getItem(prefix + ".entryformat");

    return this->getLog(name, type, enabled, keepopen); //fileformat & entryformat are optional
}

bool FileConfiguration::setCustomItem(const std::string& name, const std::string& value)
{
    return false;
}

std::string FileConfiguration::getCustomItem(const std::string& name)
{
    return this->getItem(this->getCustomOptionIdent() + "." + name);

    return "";
}

bool FileConfiguration::isCustomItemSet(const std::string& name)
{
    return this->isItemSet(this->getCustomOptionIdent() + "." + name);
}

bool FileConfiguration::setCustomCommandItem(const std::string& command, const std::string& name, const std::string& value)
{
    return false;
}

std::string FileConfiguration::getCustomCommandItem(const std::string& command, const std::string& name)
{
    std::string option = this->getCommandOptionIdent() + "." + command + "." + name;

    return this->getItem(option);
}

bool FileConfiguration::isCustomCommandItemSet(const std::string& command, const std::string& name)
{
    return this->isItemSet(this->getCommandOptionIdent() + "." + command + "." + name);
}

bool FileConfiguration::setCustomFilterItem(const std::string& filter, const std::string& name, const std::string& value)
{
    return false;
}

std::string FileConfiguration::getCustomFilterItem(const std::string& filter, const std::string& name)
{
    std::string option = this->getFilterOptionIdent() + "." + filter + "." + name;

    return this->getItem(option);
}

bool FileConfiguration::isCustomFilterItemSet(const std::string& filter, const std::string& name)
{
    return this->isItemSet(this->getFilterOptionIdent() + "." + filter + "." + name);
}

bool FileConfiguration::setXmppUser(const std::string& user, const std::string& password, const std::string& address, const std::string& resource)
{
    return false;
}

bool FileConfiguration::getXmppUser(std::string *user, std::string *password, std::string *address, std::string *resource)
{
    *user = this->getItem(this->getUserOptionIdent() + ".user");
    *password = this->getItem(this->getUserOptionIdent() + ".password");
    *address = this->getItem(this->getUserOptionIdent() + ".address");
    *resource = this->getItem(this->getUserOptionIdent() + ".resource");

    return this->isItemSet(this->getUserOptionIdent() + ".user") && this->isItemSet(this->getUserOptionIdent() + ".password")
            && this->isItemSet(this->getUserOptionIdent() + ".address") && this->isItemSet(this->getUserOptionIdent() + ".resource");
}

bool FileConfiguration::setXmppMUC(const std::string& name, const std::string& room, const std::string& server)
{
    return false;
}

bool FileConfiguration::getXmppMUC(std::string *name, std::string *room, std::string *server)
{
    *name = this->getItem(this->getMUCOptionIdent() + ".name");
    *room = this->getItem(this->getMUCOptionIdent() + ".room");
    *server = this->getItem(this->getMUCOptionIdent() + ".server");

    return this->isItemSet(this->getMUCOptionIdent() + ".name") && this->isItemSet(this->getMUCOptionIdent() + ".room")
                && this->isItemSet(this->getMUCOptionIdent() + ".server");
}
