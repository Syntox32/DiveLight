
#ifndef DIVELIGHT_UTILS_H
#define DIVELIGHT_UTILS_H

#include <string>

class Utils {
public:
    ///
    /// \param i         Bin to access
    /// \param samples   Sampling rate
    /// \param nFFT      FFT Size
    /// \return Frequency of index
    static inline float indexToFreq(int idx, float samplingRate, int fftSize)
    {
        return (float)(idx * (samplingRate / fftSize / 2.0f));
    }

    ///
    /// \param freq      Frequenzy in Hz
    /// \param samples   Sampling rate
    /// \param nFFT      FFT Size
    /// \return Index of frequency
    static inline unsigned int freqToIndex(double freq, double samplingRate, int fftSize)
    {
        return (unsigned int)(freq / (samplingRate / fftSize / 2.0f));
    }

    static inline float norm(float x, float min, float max)
    {
        return (x - min) / (max - min);
    }

    static inline float lerp(float start, float end, float t)
    {
        return (1.0f - t) * start + t * end;
    }

    static double inline map(double x, double inMin, double inMax, double outMin, double outMax) {
        return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
    }

    static inline bool stringContains(const std::string& str, const std::string& contains)
    {
        return (size_t)str.find(contains) != std::string::npos;
    }

    static float average(float *in, int min, int max)
    {
        if ((max - min) == 0) return 0;

        float res;
        for (int i = min; i < max; i++)
            res += in[i];
        return res / (max - min);
    }
};

#endif //DIVELIGHT_UTILS_H
