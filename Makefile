CC=g++
CFLAGS=-c -std=c++0x -O3 -Iinclude -Wall
CDB_FLAGS=
LBASE_FLAGS=-lgloox -lboost_program_options -lboost_date_time -lboost_regex -lpthread
LDB_FLAGS=-lsoci_core
LDB_SQLITE_FLAGS=-lsoci_sqlite3
LDB_MYSQL_FLAGS=-lsoci_mysql

EXECUTABLE=bin/xmppbot

#---- Sources
BASE_SOURCES=src/BotCommand.cpp src/BotCommandInfo.cpp src/BotCommandManager.cpp src/Configuration.cpp src/ConfigurationBased.cpp src/ConsoleLog.cpp src/FileConfiguration.cpp src/FileLog.cpp src/Log.cpp src/LogHelper.cpp src/MessageFilter.cpp src/MessageInfo.cpp src/Program.cpp src/StringFormat.cpp src/VoidLog.cpp src/XmppBot.cpp src/main.cpp
COMMAND_SOURCES=src/AdminBotCommand.cpp src/AliasBotCommand.cpp src/ConfigurationBotCommand.cpp src/HelpBotCommand.cpp src/KickBotCommand.cpp src/ProtectedBotCommand.cpp src/StateBotCommand.cpp src/SubjectBotCommand.cpp src/TestBotCommand.cpp
FILTER_SOURCES=src/CommandMessageFilter.cpp src/ForeignMessageFilter.cpp src/HistoryMessageFilter.cpp src/JIDMessageFilter.cpp src/LinkMessageFilter.cpp src/LogMessageFilter.cpp
DB_SOURCES=src/DbConfiguration.cpp src/DbLog.cpp
DB_SQLITE_SOURCES=src/SQLiteConfiguration.cpp src/SQLiteLog.cpp
DB_MYSQL_SOURCES=src/MySQLConfiguration.cpp src/MySQLLog.cpp

#---- Objects
BASE_OBJECTS=$(BASE_SOURCES:.cpp=.o)
COMMAND_OBJECTS=$(COMMAND_SOURCES:.cpp=.o)
FILTER_OBJECTS=$(FILTER_SOURCES:.cpp=.o)
DB_OBJECTS=$(DB_SOURCES:.cpp=.o)
DB_SQLITE_OBJECTS=$(DB_SQLITE_SOURCES:.cpp=.o)
DB_MYSQL_OBJECTS=$(DB_MYSQL_SOURCES:.cpp=.o)

#--- Targets

all: CDB_FLAGS+=-DDB_SUPPORT -DDB_SQLITE_SUPPORT -DDB_MYSQL_SUPPORT -I/usr/include/soci -I/usr/include/mysql -I/usr/local/include/soci -I/usr/local/include/mysql
all: base_obj db_obj mysql_obj sqlite_obj
	$(CC) $(LBASE_FLAGS) $(LDB_FLAGS) $(LDB_MYSQL_FLAGS) $(LDB_SQLITE_FLAGS) $(BASE_OBJECTS) $(COMMAND_OBJECTS) $(FILTER_OBJECTS) $(DB_OBJECTS) $(DB_MYSQL_OBJECTS) $(DB_SQLITE_OBJECTS) -o $(EXECUTABLE)

base: base_obj
	$(CC) $(LBASE_FLAGS) $(BASE_OBJECTS) $(COMMAND_OBJECTS) $(FILTER_OBJECTS) -o $(EXECUTABLE)

mysql: CDB_FLAGS+=-DDB_SUPPORT -DDB_MYSQL_SUPPORT -I/usr/include/soci -I/usr/include/mysql -I/usr/local/include/soci -I/usr/local/include/mysql
mysql: base_obj db_obj mysql_obj
	$(CC) $(LBASE_FLAGS) $(LDB_FLAGS) $(LDB_MYSQL_FLAGS) $(BASE_OBJECTS) $(COMMAND_OBJECTS) $(FILTER_OBJECTS) $(DB_OBJECTS) $(DB_MYSQL_OBJECTS) -o $(EXECUTABLE)

sqlite: CDB_FLAGS+=-DDB_SUPPORT -DDB_SQLITE_SUPPORT -I/usr/include/soci -I/usr/local/include/soci
sqlite: base_obj db_obj sqlite_obj
	$(CC) $(LBASE_FLAGS) $(LDB_FLAGS) $(LDB_SQLITE_FLAGS) $(BASE_OBJECTS) $(COMMAND_OBJECTS) $(FILTER_OBJECTS) $(DB_OBJECTS) $(DB_SQLITE_OBJECTS) -o $(EXECUTABLE)

mysql_obj: $(DB_MYSQL_SOURCES) $(DB_MYSQL_OBJECTS)

sqlite_obj: $(DB_SQLITE_SOURCES) $(DB_SQLITE_OBJECTS)

db_obj: $(DB_SOURCES) $(DB_OBJECTS)

base_obj: $(BASE_SOURCES) $(COMMAND_SOURCES) $(FILTER_SOURCES) $(BASE_OBJECTS) $(COMMAND_OBJECTS) $(FILTER_OBJECTS)

.cpp.o:
	$(CC) $(CFLAGS) $(CDB_FLAGS) $< -o $@

clean:
	rm -r src/*.o
