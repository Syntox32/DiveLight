
#include "../include/visualizer.hpp"

Visualizer::Visualizer(Config& config, SoundInput *stream)
    : m_config(config), m_stream(stream)
{

}

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
                temp = (float)20.0f * log10(inRaw[i]);
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