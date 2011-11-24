#include "XmppBot.h"

int main(int argc, char** argv)
{
	std::string cfgfile = "bot.cfg";
	if(argc > 1)
		cfgfile = std::string(argv[1]);

	XmppBot::ExitState state = XmppBot::QUIT;

	do
	{
		XmppBot* b = new XmppBot(cfgfile);
		state = b->run();

		delete b; //call dtor

	} while(state < XmppBot::QUIT);

    return state;
}
