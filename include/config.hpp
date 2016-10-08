
#ifndef DIVELIGHT_CONFIG_H
#define DIVELIGHT_CONFIG_H

#include <iostream>
#include "fft.hpp"

#ifdef _WIN32
#elif __linux__
#endif


enum ScalingMethod
{
    Decibel = 0,
    Log = 1,
    Linear = 2,
    SquareRoot = 3,
    None = 4
};

enum FreqLimitMethod {
    FreqLinear = 0,
    FreqLog = 1,
    LogV2 = 2
};

enum WindowFunction {
    Hann = 0,
    Hamming = 1,
    Blackman = 2,
    WindowNone = 3
};

enum ColorMode {
    Single = 0,
    Gradient = 1,
    Custom = 2
};

enum FFTSize : unsigned int {
    FFT512 = 512,
    FFT1024 = 1024,
    FFT2048 = 2048,
    FFT4096 = 4096,
};


struct Config {
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
    bool applySqrtPostFFT;

    unsigned int columnCount;
    unsigned int sampleIndexShift;
    unsigned int historyAvgCount;
    unsigned int renderFrameRate;
    unsigned int ledRefreshRate;

    float decayConstant;
    float linearScalingConstant;
    float linearInterpConstant;

    Config() :
        scalingMethod(ScalingMethod::SquareRoot),
        freqLimitMethod(FreqLimitMethod::FreqLog),
        windowFunction(WindowFunction::Blackman),
        colorMode(ColorMode::Single),
        fftSize(FFTSize::FFT2048),

        decayEnabled(true),
        historyAverageEnabled(true),
        sampleShiftingEnabled(false),
        bandpassFilterEnabled(false),
        beatDetectionEnabled(false),
        ledStripEnabled(false),
        linearInterpEnabled(false),
        applySqrtPostFFT(true),

        columnCount(24),
        sampleIndexShift(0),
        historyAvgCount(20),
        renderFrameRate(25),
        ledRefreshRate(60),

        decayConstant(0.6f),
        linearScalingConstant(5.0f),
        linearInterpConstant(0.6f)
    { }
};


#endif //DIVELIGHT_CONFIG_H
