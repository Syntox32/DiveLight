
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

#include <locale>
#include <codecvt>

#include <string>
#include <cmath>
#include <climits>

#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include <kiss_fft.h>
#include "stream.hpp"
#include "renderer.hpp"
#include "columnrenderer.hpp"

class DiveClient
{
public:
    DiveClient();
    void begin();
    void step(int sampleSize, int currentSample);
private:
    DiveFFT*    m_fft;
    DiveStream* m_stream;
    kiss_fft_scalar* m_dataIn;
    kiss_fft_cpx*    m_dataOut;
    float*       m_ampBuff;
    float*       m_magBuff;
    unsigned int m_magBuffLen;

    unsigned int m_channelCount;
    unsigned int m_sampleRate;
    uint64_t     m_sampleCount;

    IRenderer*   m_renderer;

    sf::RenderWindow *m_window;
    std::vector<sf::RectangleShape>* m_rects;
};


#endif //DIVELIGHT_DIVECLIENT_H
