#include "XmppBot.h"

int main(int argc, char** argv)
{
    XmppBot* b = new XmppBot("bot.cfg");
    b->run();

    delete b; //call dtor

    return 0;
}
