
#ifndef DIVELIGHT_COLUMNRENDERER_HPP
#define DIVELIGHT_COLUMNRENDERER_HPP

#include "renderer.hpp"
#include "ledclient.hpp"
#include "utils.hpp"
#include "fft.hpp"

class ColumnRenderer : public IRenderer
{
public:
    ColumnRenderer(unsigned int columnCount, std::mutex& lock);
    virtual void setData();
    virtual void render();
    void setDataPointer(float *data, unsigned int samplingRate, std::size_t len, int fftSize);
    void attachLeds(LEDClient* ledclient);
private:
    unsigned int m_columnCount;
    float        m_columnWidth;
    float*       m_dataPointer;
    std::mutex& m_lock;
    std::size_t m_dataLen;
    LEDClient  *m_led;
    bool        b_hasLeds;

    unsigned int m_freqMax;
    unsigned int m_freqMin;
};


#endif //DIVELIGHT_COLUMNRENDERER_HPP
