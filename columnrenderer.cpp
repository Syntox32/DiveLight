
#include "columnrenderer.hpp"

ColumnRenderer::ColumnRenderer(unsigned int columnCount)
    : m_columnCount(columnCount)
{
    m_rects->clear();
    float columnWidth = m_window->getSize().x / (float)m_columnCount;
    float columnHeight = 20.0f;
    //sf::Vector2u winSize = m_window->getSize();
    sf::Vector2f size(columnWidth, columnHeight);
    sf::RectangleShape temp;
    for (unsigned int i = 0; i < m_columnCount; i++)
    {
        temp = sf::RectangleShape(sf::Vector2f(columnWidth * i, m_window->getSize().y - columnHeight));
        temp.setFillColor(sf::Color::Red);
        temp.setSize(size);
        m_rects->push_back(temp);
    }
}

void ColumnRenderer::setData(float *data)
{

}