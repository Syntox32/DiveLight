
#ifndef DIVELIGHT_COLUMNRENDERER_HPP
#define DIVELIGHT_COLUMNRENDERER_HPP

#include "renderer.hpp"
#include "ledclient.hpp"
#include "utils.hpp"
#include "fft.hpp"
#include "visualizer.hpp"

class BoxSpectrum
{
public:
    BoxSpectrum(unsigned int columnCount, sf::Vector2u winSize, std::mutex& lock);
    void prepareData(float *inData, unsigned int inLen, Visualizer *vz);
    void render(sf::RenderTarget *target);

    void setFreqLimits(unsigned int minFreq,
                        unsigned int maxFreq,
                        unsigned int sampleRate,
                        unsigned int fftSize);

private:
    unsigned int m_columnCount;
    float        m_columnWidth;
    float*       m_dataPointer;
    std::mutex& m_lock;
    std::size_t m_dataLen;
    sf::Vector2u m_winSize;
    unsigned int m_freqMax;
    unsigned int m_freqMin;
    std::vector<sf::RectangleShape*> m_rects;
};


#endif //DIVELIGHT_COLUMNRENDERER_HPP
