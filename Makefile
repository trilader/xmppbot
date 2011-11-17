all:
	 g++ src/*.cpp -std=c++0x -Iinclude -lgloox -lboost_program_options -lboost_date_time -lboost_regex -lpthread -Wall -o bin/xmppbot

clean:
	rm -r *.o
