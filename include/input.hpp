
#ifndef DIVELIGHT_INPUT_HPP
#define DIVELIGHT_INPUT_HPP

#include "utils.hpp"
#include "fft.hpp"
#include "stream.hpp"
#include <thread>
#include <mutex>
#include <climits>
#include <experimental/filesystem>
#include <SFML/Graphics.hpp>

enum InputFormat : int
{
    File = 0,
    Loopback = 1
};

typedef float Sample;

class SoundInput {

public:
    SoundInput(DataCallback& dataCallback);
    ~SoundInput();

    void setFile(const std::string& path, FFTSize fftInputSize = FFTSize::FFT2048);
    void setLoopback();
    void begin();
    void stop();

    const std::vector<Sample>& getSampleData(unsigned int sampleSize, unsigned int currentSample);

    const inline bool isRunning() const { return b_running; }
    const inline unsigned int getSampleRate() const { return m_stream->getSampleRate(); }

    void render(sf::RenderTarget *target);
    void update(sf::RenderWindow *window);

private:
    bool canTogglePause;
    bool canForward;
    bool canBackward;
    bool invalidateProgressShape;
    sf::RectangleShape *progress;
    sf::RectangleShape *backdrop;
    sf::Time trackDuration;

    DiveStream*   m_stream;
    InputFormat   m_format;
    bool          b_running;
    bool          b_hasCallback;
    FFTSize       m_fftSize;
    std::thread   m_streamThread;
    std::mutex    m_streamLock;
    DataCallback& f_eventCallback;
    std::vector<Sample> m_samples;

    void beginFileStream();
    std::string prepareTrack(const std::string& path);
};


#endif //DIVELIGHT_INPUT_HPP
