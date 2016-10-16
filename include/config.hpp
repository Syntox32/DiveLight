
#ifndef DIVELIGHT_CONFIG_H
#define DIVELIGHT_CONFIG_H

#include <iostream>
#include "fft.hpp"

#ifdef _WIN32
#elif __linux__
#endif


enum ScalingMethod {
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

    bool simpleAverage;
    bool decayEnabled;
    bool historyAverageEnabled;
    bool sampleShiftingEnabled;
    bool bandpassFilterEnabled;
    bool beatDetectionEnabled;
    bool ledStripEnabled;
    bool linearInterpEnabled;
    bool applySqrtPostFFT;
    bool enableBeatDetect;

    int columnCount;
    unsigned int sampleIndexShift;
    unsigned int historyAvgCount;
    unsigned int renderFrameRate;
    unsigned int ledRefreshRate;
    int freqLow;
    int freqHigh;

    float decayConstant;
    float linearScalingConstant;
    float linearInterpConstant;
    float lerpConstant;
    float moveDelta;

    float freqLogPower;
    float freqLogCount;

    Config() :
        scalingMethod(ScalingMethod::SquareRoot),
        freqLimitMethod(FreqLimitMethod::FreqLog),
        windowFunction(WindowFunction::Blackman),
        colorMode(ColorMode::Single),
        fftSize(FFTSize::FFT4096),

        simpleAverage(false),
        decayEnabled(true),
        historyAverageEnabled(true),
        sampleShiftingEnabled(false),
        bandpassFilterEnabled(false),
        beatDetectionEnabled(false),
        ledStripEnabled(true),
        linearInterpEnabled(false),
        applySqrtPostFFT(false),
        enableBeatDetect(true),

        columnCount(12),
        sampleIndexShift(0),
        historyAvgCount(50),
        renderFrameRate(60),
        ledRefreshRate(60),

        freqLow(80),
        freqHigh(4000),

        decayConstant(0.3f),
        linearScalingConstant(0.5f),
        linearInterpConstant(0.6f),
        freqLogCount(12.0f),
        freqLogPower(2.0f),
        lerpConstant(0.7f),
        moveDelta(4.0f)
    { }
};


#endif //DIVELIGHT_CONFIG_H
