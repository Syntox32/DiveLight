
#ifndef DIVELIGHT_FFT_H
#define DIVELIGHT_FFT_H

#include <cmath>
#define PI 3.14159265359f

#include "config.hpp"
#include "kiss_fftr.h"


class DiveFFT
{
public:
    DiveFFT(unsigned int fftSize);
    ~DiveFFT();

    void setData(kiss_fft_scalar* in, kiss_fft_cpx* out, unsigned int inLen);
    void doFFT();
    inline unsigned int getFFTSize() const { return (unsigned int)m_size; }
    unsigned int getOutDataCount();

    float windowHann();
    float windowHamming();
    float windowBlackman();

private:
    unsigned int     m_size;
    kiss_fftr_cfg    m_cfg;
    kiss_fft_cpx*    m_outData;
    kiss_fft_scalar* m_inData;
    unsigned int     m_inLen;

};


#endif //DIVELIGHT_FFT_H
