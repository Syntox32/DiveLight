
#ifndef DIVELIGHT_CONFIG_H
#define DIVELIGHT_CONFIG_H

#include <iostream>
#include "fft.hpp"

#ifdef _WIN32
#elif __linux__
#endif


enum class ScalingMethod
{
    Decibel,
    Log,
    Linear,
    SquareRoot,
    None
};

enum class FreqLimitMethod {
    Linear,
    Log
};

enum class WindowFunction {
    Hann,
    Hamming,
    Blackman,
    None
};

enum class ColorMode {
    Single,
    Gradient,
    Custom
};

enum FFTSize : unsigned int {
    FFT512 = 512,
    FFT1024 = 1024,
    FFT2048 = 2048,
    FFT4096 = 4096,
};


class Config {
    ScalingMethod scalingMethod;
    FreqLimitMethod freqLimitMethod;
    WindowFunction windowFunction;
    ColorMode colorMode;
    FFTSize fftSize;

    bool decayEnabled;
    bool historyAverageEnabled;
    bool sampleShiftingEnabled;
    bool bandpassFilterEnabled;
    bool beatDetectionEnabled;
    bool ledStripEnabled;
    bool linearInterpEnabled;

    unsigned int columnCount;
    unsigned int sampleIndexShift;
    unsigned int historyAvgCount;
    unsigned int renderFrameRate;
    unsigned int ledRefreshRate;

    float decayConstant;
    float linearScalingConstant;
    float linearInterpConstant;

    Config() :
        scalingMethod(ScalingMethod::Decibel),
        freqLimitMethod(FreqLimitMethod::Linear),
        windowFunction(WindowFunction::Blackman),
        colorMode(ColorMode::Single),
        fftSize(FFTSize::FFT2048),

        decayEnabled(true),
        historyAverageEnabled(true),
        sampleShiftingEnabled(false),
        bandpassFilterEnabled(false),
        beatDetectionEnabled(false),
        ledStripEnabled(true),
        linearInterpEnabled(false),

        columnCount(30),
        sampleIndexShift(0),
        historyAvgCount(20),
        renderFrameRate(120),
        ledRefreshRate(60),

        decayConstant(0.6f),
        linearScalingConstant(5.0f),
        linearInterpConstant(0.6f)
    { }
};


#endif //DIVELIGHT_CONFIG_H
