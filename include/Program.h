#ifndef PROGRAM_H
#define PROGRAM_H

#include "Configuration.h"
#include "XmppBot.h"
#include "FileConfiguration.h"
#include "SQLiteConfiguration.h"
#include "MySQLConfiguration.h"
#include "LogHelper.h"

#include "boost/program_options.hpp"
#include <string>

#include "boost/algorithm/string.hpp"

namespace bpo = boost::program_options;

class Program
{
    public:
        static int main(int argc, char** argv);

    private:
        static void parseOptions(int argc, char** argv);
        static bool loadConfiguration();
        static int runBot();
        static int editConfiguration();

        static Configuration *_config;
        static bpo::variables_map _vm;
};

#endif
