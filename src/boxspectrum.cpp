
#include "../include/boxspectrum.hpp"

BoxSpectrum::BoxSpectrum(unsigned int columnCount, sf::Vector2u winSize, std::mutex& lock)
    : m_columnCount(columnCount), m_lock(lock), m_winSize(winSize)
{
    m_columnWidth = winSize.x / (float)m_columnCount;

    m_averages = new float[m_columnCount];
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

/*
void BoxSpectrum::setFreqLimits(unsigned int minFreq,
                                unsigned int maxFreq,
                                Visualizer *vz)
{
    m_freqMin = vz->freqToIndex(minFreq);
    m_freqMax = vz->freqToIndex(maxFreq);
    //m_freqMin = Utils::freqToIndex(minFreq, sampleRate, fftSize);
    //m_freqMax = Utils::freqToIndex(maxFreq, sampleRate, fftSize);
}
*/

void BoxSpectrum::prepareData(float *inData, unsigned int inLen, Visualizer *vz, Config& config)
{
    int averages = vz->getAverages(inData, config.columnCount, m_averages, config.freqLow, config.freqHigh);
    float lastAvg = 0.0f; // used by simpleAverage

    for (int i = 0; i < averages; i++)
    {
        float avg = m_averages[i];

        if (config.simpleAverage) {
            if (i != 0) {
                avg = (lastAvg + avg) / 2.0f;
            }

            lastAvg = avg;
        }

        float lastH = m_rects[i]->getSize().y;
        float h;

        if (config.decayEnabled) {
            if (avg > lastH)
                h = Utils::lerp(lastH, avg, config.lerpConstant);
            else if (avg < lastH)
                h = Utils::lerp(lastH, avg, config.decayConstant);
            else
                h = avg;
        } else {
            h = Utils::lerp(lastH, avg, config.lerpConstant);
        }

        h = Utils::norm(h, 0, 2500)*600;

        m_rects[i]->setPosition(m_columnWidth * i + (i * 5), m_winSize.y - h);
        m_rects[i]->setSize(sf::Vector2f(m_columnWidth, h));
    }
}


void BoxSpectrum::render(sf::RenderTarget *target)
{
    for (size_t i = 0; i < m_rects.size(); i++)
    {
        target->draw(*m_rects[i]);
    }
}