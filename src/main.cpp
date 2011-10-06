#include "XmppBot.h"

int main(int argc, char** argv)
{
    XmppBot* b = new XmppBot();
    b->run();

    delete b; //call dtor

    return 0;
}
