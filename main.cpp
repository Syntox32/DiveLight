
/*
 * #### DiveLight ####
 *
 *  Anders Milje (github.com/syntox32) 2016 - MIT Licence.
 *
 *
 * -- The plan --
 *
 * Two parts: server and a client. Client being your computer, and server being a raspberry pi (2 or 3).
 *
 * Client:
 *  1. Establish connection to server (using web-sockets).
 *  2. User requests a song.
 *  3. File is converted/downloaded.
 *  4. Begin loop:
 *      1. Do FFT and beat-detection. (Heavy calculations).
 *      2. Send LED-state updates to server (RPi).
 *
 *  Server:
 *   1. Begin web-socket loop:
 *      1. Wait for connection.
 *      2. Receive data (configs, data, etc..)
 *      3. Initialize thread-safe LED state object.
 *      4. Start LED update thread
 *   2. Begin LED thread (running at 60 to 120 fps):
 *      1. Access LED state
 *      2. Update if needed
 *      3. Send update to RPI GPIO
 *
 *
 * -- The Libraries --
 *
 *  The goal is for this to work cross-platform.
 *
 *  Shared:
 *   - Web-sockets:         https://github.com/zaphoyd/websocketpp
 *
 *  Client:
 *   - FFT Library:         https://github.com/FFTW/fftw3
 *
 *  Server (RPi):
 *   - LED Interaction:     https://github.com/jgarff/rpi_ws281x
 *
 * */

#include <iostream>
#include "client.hpp"
#include "server.hpp"

void printHelp() {
    std::cout << "Please launch as either 'dive.exe server' or 'dive.exe client'" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printHelp();
    }

    char choice = (char)std::tolower((unsigned  char)argv[1][0]);
    switch (choice) {
        case 's': {
            DiveServer *s = new DiveServer();
            s->begin();
            delete s;
            break;
        }
        case 'c': {
            DiveClient *c = new DiveClient();
            c->begin();
            delete c;
            break;
        }
        default:
            printHelp();
            break;
    }

    return 0;
}