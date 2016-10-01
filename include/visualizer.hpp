
#ifndef DIVELIGHT_VISUALIZER_HPP
#define DIVELIGHT_VISUALIZER_HPP

/**
   Should be able to:
    - Scale FFT output:
        - Decibel   20.0f * log10(in[i])
        - Logarithmically    10.0f * log(in[i])
        - Linear    *= 2
        - Square root   sqrt(in[i]) * 2

    - Create frequency limits:
        - Logarithmically
        - Linearly

    - Decay function
    - History average
    - Sample shifting

    - Change column count
    - Change and manage color

    - Change Window function
    - Change FFT Size during runtime (might be difficult)

    - Apply bandpass filters
    - Beat detection

    - LED Style configurator

    - Save/Load settings

 */

#include "config.hpp"
#include "input.hpp"

class Visualizer {
public:
    Visualizer(Config& config, SoundInput *stream);

    void applyPostFFTScaling(float *inRaw,
                             unsigned int inLen,
                             float *outMag,
                             float *outAmp);



private:
    Config &m_config;
    SoundInput *m_stream;
};


#endif //DIVELIGHT_VISUALIZER_HPP
