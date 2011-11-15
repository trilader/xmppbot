#ifndef JIDMAP_H
#define JIDMAP_H

#include "boost/unordered_map.hpp"
#include "gloox/client.h"

typedef boost::unordered_map<gloox::JID,gloox::JID> JIDMap;

#endif
