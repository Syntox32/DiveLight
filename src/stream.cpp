
#include "../include/stream.hpp"

void DiveStream::load(const sf::SoundBuffer& buffer, int sampleSize, DataCallback& callback)
{
    m_samples.assign(buffer.getSamples(), buffer.getSamples() + buffer.getSampleCount());
    m_currentSample = 0;
    m_samplesToStream = (size_t)sampleSize;
    m_callback = callback;

    initialize(buffer.getChannelCount(), buffer.getSampleRate());
}

bool DiveStream::onGetData(Chunk& data)
{
    data.samples = &m_samples[m_currentSample];

    if (m_currentSample + m_samplesToStream <= m_samples.size())
    {
        data.sampleCount = m_samplesToStream;
        m_currentSampleCount = m_samplesToStream;
        m_currentSample += m_samplesToStream;

        // initiate callback functions with the number of samples we just took
        m_callback((unsigned int)m_samplesToStream, (unsigned int)(m_currentSample - data.sampleCount));

        return true;
    }
    else
    {
        data.sampleCount = m_samples.size() - m_currentSample;
        m_currentSampleCount = m_samples.size() - m_currentSample;
        m_currentSample = m_samples.size();

        // initiate callback functions with the number of samples we just took
        m_callback((unsigned int)data.sampleCount, (unsigned int)(m_samples.size() - data.sampleCount));

        return false;
    }
}

void DiveStream::onSeek(sf::Time offset)
{
    m_currentSample = static_cast<std::size_t>(offset.asSeconds() * getSampleRate() * getChannelCount());
}