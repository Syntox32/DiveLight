
/*
 * #### DiveLight ####
 *
 *  Anders Milje (github.com/syntox32) 2016 - MIT Licence.
 *
 *  repo: https://github.com/Syntox32/DiveLight
 *
 * -- Plan
 *
 *  The goal is for this to work cross-platform.
 *
 * -- Libraries
 *
 *  Server(C++):
 *      - WebSocket client - https://github.com/dhbaird/easywsclient
 *      - Fourier transform - https://github.com/itdaniher/kissfft
 *      - (might use boost if I give up on string and file manipulation)
 *
 *  Client(Python >=3.5.x):
 *      - LED interaction - https://github.com/jazzycamel/ws28128-rpi
 *      - websockes
 *
 * */

#include "../include/client.hpp"

int main(int argc, char* argv[])
{
    DiveClient c;
    c.init();

    return 0;
}