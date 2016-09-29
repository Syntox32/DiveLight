
#ifndef DIVELIGHT_UTILS_H
#define DIVELIGHT_UTILS_H

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
    static inline int freqToIndex(double freq, double samplingRate, int fftSize)
    {
        return (int)(freq / (samplingRate / fftSize / 2.0f));
    }
};

#endif //DIVELIGHT_UTILS_H
