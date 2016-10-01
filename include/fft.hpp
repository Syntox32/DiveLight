
#ifndef DIVELIGHT_FFT_H
#define DIVELIGHT_FFT_H

#include <cmath>
#define PI 3.14159265359f

#include "config.hpp"
#include "kiss_fftr.h"

enum FFTSize : int
{
    FFT512 = 512,
    FFT1024 = 1024,
    FFT2048 = 2048,
    FFT4096 = 4096,
};

class DiveFFT
{
public:
    DiveFFT(FFTSize fftSize = FFTSize::FFT1024);
    ~DiveFFT();

    void setData(kiss_fft_scalar* in, kiss_fft_cpx* out, unsigned int inLen);
    void doFFT();
    inline int getFFTSize() const { return m_size; }
    unsigned int getOutDataCount();

    float windowHann();
    float windowHamming();
    float windowBlackman();

private:
    FFTSize          m_size;
    kiss_fftr_cfg    m_cfg;
    kiss_fft_cpx*    m_outData;
    kiss_fft_scalar* m_inData;
    unsigned int     m_inLen;

};


#endif //DIVELIGHT_FFT_H
