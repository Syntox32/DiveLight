
#ifndef DIVELIGHT_DIVESTREAM_H
#define DIVELIGHT_DIVESTREAM_H

#include <vector>
#include <functional>
#include "SFML/Audio.hpp"
#include "fft.hpp"

class DiveStream : public sf::SoundStream
{
public:
    void load(const sf::SoundBuffer&, int, std::function<void(int, int)>);

    const std::vector<sf::Int16>& getSamples() const { return m_samples; }
    const std::size_t& currentSample() const         { return m_currentSample; }
    const int samplesToStream() const                { return m_samplesToStream; }

private:
    virtual bool onGetData(Chunk&);
    virtual void onSeek(sf::Time);

    std::vector<sf::Int16> m_samples;
    std::size_t            m_currentSample;
    int                    m_samplesToStream;
    std::function<void(int, int)> m_callback;
};

#endif //DIVELIGHT_DIVESTREAM_H
