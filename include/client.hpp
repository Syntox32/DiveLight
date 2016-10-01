
#ifndef DIVELIGHT_DIVECLIENT_H
#define DIVELIGHT_DIVECLIENT_H

#include <vector>
#include <algorithm>

#include <cstdlib>     // system
#include <cstdio>      // perror
#include <unistd.h>    // dup2
#include <sys/types.h> // rest for open/close
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "config.hpp"
#include "fft.hpp"
#include "input.hpp"
#include  <kiss_fft.h>
#include "stream.hpp"
#include "renderer.hpp"
#include "columnrenderer.hpp"
#include "easywsclient.hpp"
#include "ledclient.hpp"

#include <locale>
#include <codecvt>

#include <string>
#include <cmath>
#include <math.h>
#include <climits>

#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

class DiveClient
{
public:
    DiveClient();
    void init();

private:
    void step(unsigned int sampleSize, unsigned int currentSample);

    LEDClient  *m_ledClient;
    SoundInput *m_soundInput;
    DiveFFT    *m_fft;
    kiss_fft_scalar *m_dataIn;
    kiss_fft_cpx    *m_dataOut;
    float       *m_ampBuff;
    float       *m_magBuff;
    unsigned int m_magBuffLen;
    ColumnRenderer* m_renderer;
    std::mutex m_lock;
};


#endif //DIVELIGHT_DIVECLIENT_H