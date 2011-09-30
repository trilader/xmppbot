#include "XmppBot.h"

int main(int argc, char** argv)
{
    XmppBot* b = new XmppBot();
    delete b;
    b = 0;
    return 0;
}
