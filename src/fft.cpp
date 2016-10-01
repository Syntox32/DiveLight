
#include "../include/fft.hpp"

DiveFFT::DiveFFT(FFTSize fftSize)
        : m_size(fftSize)
{
    m_cfg = kiss_fftr_alloc(fftSize, 0, 0, 0);
}

void DiveFFT::setData(kiss_fft_scalar* in, kiss_fft_cpx* out, unsigned int inLen)
{
    m_inData  = in;
    m_outData = out;
    m_inLen   = inLen;
}

void DiveFFT::doFFT()
{
    kiss_fftr(m_cfg, m_inData, m_outData);
}

unsigned int DiveFFT::getOutDataCount()
{
    return (m_size / 2) + 1;
}

float DiveFFT::windowHann()
{
    float mult;
    for (unsigned int i = 0; i < m_inLen; i++)
    {
        mult = 0.5f * (1.0f - (float)cos((2.0f * PI * i) / (m_inLen - 1)));
        m_inData[i] *= mult;
    }
}

float DiveFFT::windowHamming()
{
    float a = 0.54f;
    float b = 0.46f;

    float mult;
    for (unsigned int i = 0; i < m_inLen; i++)
    {
        mult = a - (b * (float)cos((2.0f * PI * i) / (m_inLen - 1)));
        m_inData[i] *= mult;
    }
}

float DiveFFT::windowBlackman()
{
    float a0 = 0.42f;
    float a1 = 0.50f;
    float a2 = 0.08f;

    float mult;
    for (unsigned int i = 0; i < m_inLen; i++)
    {
        mult = a0 - a1 * (float)cos((2.0f * PI * i) / (m_inLen - 1)) + a2 * (float)cos((4.0f * PI * i) / (m_inLen - 1));
        m_inData[i] *= mult;
    }
}

DiveFFT::~DiveFFT()
{
    kiss_fftr_free(m_cfg);
}