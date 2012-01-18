all:
	 g++ src/*.cpp -std=c++0x -O3 -Iinclude -lgloox -lboost_program_options -lboost_date_time -lboost_regex -lpthread -lsqlite3 -lsqlitewrapped -Wall -o bin/xmppbot

clean:
	rm -r src/*.o
