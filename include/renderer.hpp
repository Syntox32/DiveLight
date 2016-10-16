
#ifndef DIVELIGHT_RENDERER_H
#define DIVELIGHT_RENDERER_H

#include <vector>
#include <thread>
#include <mutex>
#include <functional>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "config.hpp"
#include "ledclient.hpp"
#include "visualizer.hpp"
#include "menu.hpp"
#include "input.hpp"
#include "fft.hpp"
#include "boxspectrum.hpp"
#include "beatdetection.hpp"

class BoxSpectrum;

class Renderer {

public:
    Renderer(Config& clientConfig, SoundInput *stream, DiveFFT *fft, std::mutex& lock);

    void render();
    void begin();
    void loop();
    void prepareData();

    void setDataPointer(float *data);
    void attachLeds(LEDClient *ledClient);
    void attachBeatDetect(BeatDetection *beatDetect);

protected:
    float *m_rawData;
    float *m_ampBuff;
    float *m_magBuff;
    bool m_invalidateFreqs;

    std::mutex &m_lock;
    Config &m_config;

    BoxSpectrum *m_box;
    BeatDetection *m_beatDetect;
    SoundInput *m_stream;
    Visualizer *m_visualizer;
    DiveFFT *m_fft;
    Menu *m_menu;
    LEDClient *m_led;
    sf::RenderWindow  *m_window;
};


#endif //DIVELIGHT_RENDERER_H
