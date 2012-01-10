#include "Configuration.h"

bool Configuration::setFromSeperatedString(const std::string& optionstr, const std::string& value, std::string seperator)
{
    if(!this->isWritable())
        return false;

    std::vector<std::string> parts;
    boost::algorithm::split(parts, optionstr, boost::is_any_of(seperator));

    if(parts.size() < 2)
        return false;

    std::string ident = parts[0];
    parts.erase(parts.begin());

    if(ident == this->getCustomOptionIdent())
    {
        return this->setCustomItem(boost::algorithm::join(parts, seperator), value);
    }

    if(ident == this->getCommandOptionIdent())
    {
        if(parts.size() < 2)
            return false;

        std::string command = parts[0];
        parts.erase(parts.begin());

        return this->setCustomCommandItem(command, boost::algorithm::join(parts, seperator), value);
    }

    if(ident == this->getFilterOptionIdent())
    {
        if(parts.size() < 2)
            return false;

        std::string filter = parts[0];
        parts.erase(parts.begin());

        return this->setCustomFilterItem(filter, boost::algorithm::join(parts, seperator), value);
    }

    if(ident == this->getMUCOptionIdent())
    {
        if(parts.size() > 1)
            return false;

        std::string name, server, room;
        this->getXmppMUC(&name, &room, &server);

        if("name" == parts[0])
            name = value;
        else if("server" == parts[0])
            server = value;
        else if("room" == parts[0])
            room = value;
        else
            return false;

        return this->setXmppMUC(name, room, server);
    }

    if(ident == this->getUserOptionIdent())
    {
        if(parts.size() > 1)
            return false;

        std::string user, password, address, resource;
        this->getXmppUser(&user, &password, &address, &resource);

        if("user" == parts[0])
            user = value;
        else if("password" == parts[0])
            password = value;
        else if("address" == parts[0])
            address = value;
        else if("resource" == parts[0])
            resource = value;
        else
            return false;

        return this->setXmppUser(user, password, address, resource);
    }

    if(ident == this->getLogOptionIdent())
    {
        if(parts.size() != 2)
            return false;

        std::string log = parts[0];
        parts.erase(parts.begin());

        bool enabled, keepopen;
        std::string fileformat, entryformat, type;

        this->getLog(log, &type, &fileformat, &entryformat, &enabled, &keepopen);

        if("type" == parts[0])
            type = value;
        else if("fileformat" == parts[0])
            fileformat = value;
        else if("entryformat" == parts[0])
            entryformat = value;
        else if("enabled" == parts[0])
            enabled = value == "yes";
        else if("keepopen" == parts[0])
            keepopen = value == "yes";
        else
            return false;

        return this->setLog(log, type, fileformat, entryformat, enabled, keepopen);
    }

    return false;
}

bool Configuration::getFromSeperatedString(const std::string& optionstr, std::string *value, std::string seperator)
{
    std::vector<std::string> parts;
    boost::algorithm::split(parts, optionstr, boost::is_any_of(seperator));

    *value = "";

    if(parts.size() < 1)
        return false;

    std::string ident = parts[0];
    parts.erase(parts.begin());

    if(ident == this->getCustomOptionIdent())
    {
        if(parts.size() < 1)
            return false;

        *value = this->getCustomItem(boost::algorithm::join(parts, seperator));
        return true;
    }

    if(ident == this->getCommandOptionIdent())
    {
        if(parts.size() < 2)
            return false;

        std::string command = parts[0];
        parts.erase(parts.begin());

        *value = this->getCustomCommandItem(command, boost::algorithm::join(parts, seperator));
        return true;
    }

    if(ident == this->getFilterOptionIdent())
    {
        if(parts.size() < 2)
            return false;

        std::string filter = parts[0];
        parts.erase(parts.begin());

        *value = this->getCustomFilterItem(filter, boost::algorithm::join(parts, seperator));
        return true;
    }

    if(ident == this->getMUCOptionIdent())
    {
        if(parts.size() > 1)
            return false;

        std::string name, server, room;
        this->getXmppMUC(&name, &room, &server);

        if(parts.size() < 1)
        {
            *value = "( name: '" + name + "', room: '" + room + "', server: '" + server +"' )";
            return true;
        }

        if("name" == parts[0])
            *value = name;
        else if("server" == parts[0])
            *value = server;
        else if("room" == parts[0])
            *value = room;
        else
            return false;

        return true;
    }

    if(ident == this->getUserOptionIdent())
    {
        if(parts.size() > 1)
            return false;

        std::string user, password, address, resource;
        this->getXmppUser(&user, &password, &address, &resource);

        if(parts.size() < 1)
        {
            *value = "( user: '" + user + "', password: '" + password + "', address: '" + address +"', resource: '" + resource + "' )";
            return true;
        }

        if("user" == parts[0])
            *value = user;
        else if("password" == parts[0])
            *value = password;
        else if("address" == parts[0])
            *value = address;
        else if("resource" == parts[0])
            *value = resource;
        else
            return false;

        return true;
    }

    if(ident == this->getLogOptionIdent())
    {
        if(parts.size() < 1 || parts.size() > 2)
            return false;

        std::string log = parts[0];
        parts.erase(parts.begin());

        bool enabled, keepopen;
        std::string fileformat, entryformat, type;

        this->getLog(log, &type, &fileformat, &entryformat, &enabled, &keepopen);

        if(parts.size() < 1)
        {
            *value = "( log: '" + log + "', type: '" + type + "', fileformat: '" + fileformat +"', entryformat: '" + entryformat + "', "
                        + "enabled: '" + (enabled ? "yes" : "no") + "', keepopen: '" + (keepopen ? "yes" : "no") + "' )";
            return true;
        }

        if("type" == parts[0])
            *value = type;
        else if("fileformat" == parts[0])
            *value = fileformat;
        else if("entryformat" == parts[0])
            *value = entryformat;
        else if("enabled" == parts[0])
            *value = enabled ? "yes" : "no";
        else if("keepopen" == parts[0])
            *value = keepopen ? "yes" : "no";
        else
            return false;

        return true;
    }

    return false;
}
