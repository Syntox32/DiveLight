
#ifndef DIVELIGHT_CONFIG_H
#define DIVELIGHT_CONFIG_H

#include <iostream>

#ifdef _WIN32

#elif __linux__
    // linux
#elif __unix__ // all unices not caught above
    // Unix
#elif defined(_POSIX_VERSION)
    // POSIX
#else
#   error "Unknown compiler"
#endif

#define CACHE_CONVERTED_TRACKS true

class config {

};


#endif //DIVELIGHT_CONFIG_H
