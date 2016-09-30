
#include "columnrenderer.hpp"

ColumnRenderer::ColumnRenderer(unsigned int columnCount, std::mutex& lock)
    : IRenderer(lock), m_columnCount(columnCount), m_lock(lock), b_hasLeds(false)
{
    m_window->setFramerateLimit(120);
    m_rects->clear();
    m_columnWidth = m_window->getSize().x / (float)m_columnCount;

    float columnHeight = 20.0f;
    //sf::Vector2u winSize = m_window->getSize();
    sf::Vector2f size(m_columnWidth, columnHeight);
    sf::RectangleShape temp;

    for (unsigned int i = 0; i < m_columnCount; i++)
    {
        temp = sf::RectangleShape(); //sf::Vector2f());
        temp.setFillColor(sf::Color::Red);
        temp.setSize(size);
        temp.setPosition(m_columnWidth * i + (i * 2), m_window->getSize().y - columnHeight);

        m_rects->push_back(temp);
    }
}

void ColumnRenderer::setDataPointer(float *data, unsigned int sampleRate, std::size_t dataLen, int fftSize)
{
    m_dataPointer = data;
    m_dataLen = dataLen;

    m_freqMin = Utils::freqToIndex(40.0, sampleRate, fftSize);
    m_freqMax = Utils::freqToIndex(2000.0, sampleRate, fftSize);
}

void ColumnRenderer::setData()
{
    unsigned int freqsPerColumn = (unsigned int)((abs(m_freqMax - m_freqMin)) / m_columnCount);
    float lastAvg = 0.0f;
    float avg = 0.0f;
    float max = 0.0f;
    for (unsigned int i = 0; i < m_columnCount; i++)
    {
        avg = 0.0f;
        std::lock_guard<std::mutex> lock_guard(m_lock);
        for (unsigned int k = m_freqMin + (i * freqsPerColumn); k < m_freqMin + ((i + 1) * freqsPerColumn); k++)
        {
            avg += m_dataPointer[k];
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
        m_rects->at(i).setPosition(m_columnWidth * i + (i * 5), m_window->getSize().y - h);
        m_rects->at(i).setSize(sf::Vector2f(m_columnWidth, h));
    }

    //std::cout << "max: " << max << std::endl;
}

void ColumnRenderer::attachLeds(LEDClient *client)
{
    m_led = client;
    b_hasLeds = true;
}

void ColumnRenderer::render()
{
    setData();

    if (b_hasLeds)
    {
        std::lock_guard<std::mutex> lock_guard(m_lock);
        m_led->setData(m_dataPointer, 12, 255, 0, 0);
        m_led->send();
    }

    m_window->clear(sf::Color::Black);

    for (size_t i = 0; i < m_rects->size(); i++)
    {
        m_window->draw(m_rects->at(i));
    }

    m_window->display();
    if (b_hasLeds) m_led->poll();

}