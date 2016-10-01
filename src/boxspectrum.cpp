
#include "../include/boxspectrum.hpp"

BoxSpectrum::BoxSpectrum(unsigned int columnCount, sf::Vector2u winSize, std::mutex& lock)
    : m_columnCount(columnCount), m_lock(lock), m_winSize(winSize)
{
    m_columnWidth = winSize.x / (float)m_columnCount;

    float columnHeight = 20.0f;
    sf::Vector2f size(m_columnWidth, columnHeight);
    sf::RectangleShape *temp;

    for (unsigned int i = 0; i < m_columnCount; i++)
    {
        temp = new sf::RectangleShape(); //sf::Vector2f());
        temp->setFillColor(sf::Color::Red);
        temp->setSize(size);
        temp->setPosition(m_columnWidth * i + (i * 2), winSize.y - columnHeight);

        m_rects.push_back(temp);
    }
}

void BoxSpectrum::setFreqLimits(unsigned int minFreq,
                                unsigned int maxFreq,
                                unsigned int sampleRate,
                                unsigned int fftSize)
{
    m_freqMin = Utils::freqToIndex(minFreq, sampleRate, fftSize);
    m_freqMax = Utils::freqToIndex(maxFreq, sampleRate, fftSize);
}

void BoxSpectrum::prepareData(float *inData, unsigned int inLen, Visualizer *vz)
{
    unsigned int freqsPerColumn = (unsigned int)((abs(m_freqMax - m_freqMin)) / m_columnCount);
    float lastAvg = 0.0f;
    float avg = 0.0f;
    float max = 0.0f;
    //std::lock_guard<std::mutex> lock_guard(m_lock);
    for (unsigned int i = 0; i < m_columnCount; i++)
    {
        avg = 0.0f;
        for (unsigned int k = m_freqMin + (i * freqsPerColumn); k < m_freqMin + ((i + 1) * freqsPerColumn); k++)
        {
            avg += inData[k];
        }
        avg /= freqsPerColumn;

        if (avg > max) max = avg;

        if (i != 0) {
            avg = (lastAvg + avg) / 2.0f;
        }

        lastAvg = avg;

        //float linearScaling = (i / m_columnCount);
        //lastAvg *= linearScaling;

        float h = avg * 15;
        m_rects[i]->setPosition(m_columnWidth * i + (i * 5), m_winSize.y - h);
        m_rects[i]->setSize(sf::Vector2f(m_columnWidth, h));
    }

    //std::cout << "max: " << max << std::endl;
}


void BoxSpectrum::render(sf::RenderTarget *target)
{
    for (size_t i = 0; i < m_rects.size(); i++)
    {
        target->draw(*m_rects[i]);
    }
}