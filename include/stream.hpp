
#ifndef DIVELIGHT_DIVESTREAM_H
#define DIVELIGHT_DIVESTREAM_H

#include <vector>
#include <functional>
#include "SFML/Audio.hpp"
#include "fft.hpp"

typedef std::function<void(unsigned int, unsigned int)> DataCallback;

class DiveStream : public sf::SoundStream
{
public:
    void load(const sf::SoundBuffer&, int, DataCallback&);

    const std::vector<sf::Int16>& getSamples() const { return m_samples; }
    const std::size_t& currentSample() const         { return m_currentSample; }
    const size_t samplesToStream() const             { return m_samplesToStream; }
    const size_t currentSampleCount() const          { return m_currentSampleCount; }

private:
    virtual bool onGetData(Chunk&);
    virtual void onSeek(sf::Time);

    std::vector<sf::Int16> m_samples;
    std::size_t            m_currentSample;
    size_t                 m_samplesToStream;
    size_t                 m_currentSampleCount;
    DataCallback           m_callback;
};

#endif //DIVELIGHT_DIVESTREAM_H
