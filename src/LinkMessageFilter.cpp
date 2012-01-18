#include "LinkMessageFilter.h"

LinkMessageFilter::LinkMessageFilter(const std::string& protocols)
{
    this->_protocols = protocols;
    this->setLogFormat(new StringFormat("%1: %2"));
    std::string proto="(";

    std::vector<std::string> out;
    boost::algorithm::split(out,_protocols,boost::algorithm::is_any_of(","));

    for(unsigned int i=0; i<out.size(); i++)
    {
        proto+=out[i];
        if(i+1<out.size())
            proto+="|";
    }
    proto+=")";

    LOG(debug) << proto;

    std::string expr = proto+"://[^\\s]+";
    this->_re = new boost::regex(expr, boost::regex::icase);
}

void LinkMessageFilter::setLogFormat(StringFormat *format)
{
    this->_format = format;
}

void LinkMessageFilter::handleMessage(MessageInfo *info)
{
    std::string msg = info->getMessage().body();

    if(msg.size() <= 0)
        return;

    if(!info->isRoom() || info->isPrivate())
        return;

    JID from = info->getMessage().from();
    StringFormat *logformat = this->_format;

    boost::sregex_iterator it(msg.begin(), msg.end(), *_re);
    boost::sregex_iterator end;

    for(; it!=end; it++)
    {
        logformat->assign("1", from.resource());
        logformat->assign("2", it->str());

        LOG(link) << logformat->produce();
    }
}
