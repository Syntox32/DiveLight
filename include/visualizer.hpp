
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
#include "utils.hpp"
#include "input.hpp"
#include "fft.hpp"

class Visualizer {
public:
    Visualizer(Config& config, SoundInput *stream, DiveFFT *fft);

    void applyPostFFTScaling(float *inRaw,
                             unsigned int inLen,
                             float *outMag,
                             float *outAmp);

    //
    // This is taken from a wonderful article about FFT averages.
    // http://code.compartmental.net/2007/03/21/fft-averages/
    //
    int freqToIndex(int freq);
    int indexToFreq(int index);

    int getAverages(float *in, int len, float *out, int minFreq, int maxFreq);

private:
    Config &m_config;
    SoundInput *m_stream;
    DiveFFT *m_fft;

    int m_minFreq;
    int m_maxFreq;

    float getBandWidth();
};


#endif //DIVELIGHT_VISUALIZER_HPP
