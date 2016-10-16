
#include "../include/visualizer.hpp"

Visualizer::Visualizer(Config& config,
                       SoundInput *stream,
                       DiveFFT *fft)
    : m_config(config),
      m_stream(stream),
      m_fft(fft)
{ }

void Visualizer::applyPostFFTScaling(float *inRaw,
                                     unsigned int inLen,
                                     float *outMag,
                                     float *outAmp)
{
    if (m_config.applySqrtPostFFT)
        for (unsigned int i = 0; i < inLen; i++)
            inRaw[i] = (float)sqrt(inRaw[i]);

    float temp;
    switch (m_config.scalingMethod)
    {
        case ScalingMethod::Decibel:
            for (unsigned int i = 0; i < inLen; i++)
            {
                outMag[i] = inRaw[i];
                temp = (float)10.0f * log10(inRaw[i]);
                outAmp[i] = temp;
            }
            break;

        case ScalingMethod::Log:
            for (unsigned int i = 0; i < inLen; i++)
            {
                outMag[i] = inRaw[i];
                temp = (float)(log(inRaw[i]) * m_config.linearInterpConstant);
                outAmp[i] = temp;
            }
            break;

        case ScalingMethod::Linear:
            for (unsigned int i = 0; i < inLen; i++)
            {
                outMag[i] = inRaw[i];
                temp = (float)(inRaw[i] * m_config.linearScalingConstant);
                outAmp[i] = temp;
            }
            break;

        case ScalingMethod::SquareRoot:
            for (unsigned int i = 0; i < inLen; i++)
            {
                outMag[i] = inRaw[i];
                temp = (float)(sqrt(inRaw[i]) * m_config.linearScalingConstant);
                outAmp[i] = temp;
            }
            break;

        case ScalingMethod::None:
            for (unsigned int i = 0; i < inLen; i++)
            {
                outMag[i] = outAmp[i] = inRaw[i];
            }
            break;
    }
}

int Visualizer::getAverages(float *in,
                             int averageCount,
                             float *out,
                             int minFreq,
                             int maxFreq)
{

    switch(m_config.freqLimitMethod)
    {
        case FreqLimitMethod::FreqLinear:
        {
            int minFreqIdx = freqToIndex(minFreq);
            int maxFreqIdx = freqToIndex(maxFreq);

            int freqPerAvg = (int)abs(maxFreqIdx - minFreqIdx) / averageCount;

            for (int i = 0; i < averageCount; i++)
                out[i] =  Utils::average(in, minFreqIdx + (i * freqPerAvg), minFreqIdx + ((i + 1) * freqPerAvg));

            return averageCount;
        }
        case FreqLimitMethod::FreqLog:
        {
            //int bandsPerOctave = 12;
            //float freqPerAvg = (maxFreq - minFreq) / averageCount;
            //int minIndex = freqToIndex(minFreq);
            //int maxIndex = freqToIndex(maxFreq);
            for (int i = 0; i < averageCount; i++)
            {
                float avg;

                if (i == 0)
                    minFreq = 0;
                else
                    minFreq = (int)((m_stream->getSampleRate() / 2)
                                              / (float)pow(m_config.freqLogPower, averageCount - i));

                maxFreq = (int)((m_stream->getSampleRate() / 2)
                                / (float)pow(m_config.freqLogPower, (averageCount - 1) - i));

                int minIndex = freqToIndex(minFreq);
                int maxIndex = freqToIndex(maxFreq);
                //int indicesPerAvg = (int)((maxIndex - minIndex) / averageCount);

                avg = Utils::average(in, minIndex, maxIndex);
                out[i] = avg;
                /*for (int k = 0; k < averageCount; k++)
                {
                    int offset = k + i * averageCount;
                    float avgerageee = average(in,
                                    minIndex + (minIndex * k),
                                    minIndex + (minIndex * k) + indicesPerAvg);
                    out[offset] = avgerageee;
                }*/
            }

            return averageCount;
        }
        case FreqLimitMethod::LogV2:
        {
            int minFreqIndex = freqToIndex(minFreq);
            int maxFreqIndex = freqToIndex(maxFreq);

            int delta = abs(maxFreqIndex - minFreqIndex);

            int lower, higher;
            float avg;
            for (int i = 0; i < averageCount; i++)
            {
                out[i] = avg;
            }

            return averageCount;
        }
    }

    return -1;
}

int Visualizer::freqToIndex(int freq)
{
    if (freq < getBandWidth() / 2.0f)
        return 0;

    if (freq > m_stream->getSampleRate() / 2.0f)
        return m_fft->getFFTSize() / 2;

    float fraction = (float)freq / (float)m_stream->getSampleRate();
    int index = (int)round(m_fft->getFFTSize() * fraction);

    return index;
}

int Visualizer::indexToFreq(int index)
{
    return (int)(index * (m_stream->getSampleRate() / m_fft->getFFTSize() / 2.0f));
}

float Visualizer::getBandWidth()
{
    return (2.0f / m_fft->getFFTSize()) * (m_stream->getSampleRate() / 2.0f);
}