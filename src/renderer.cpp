
#include "../include/renderer.hpp"

IRenderer::IRenderer(std::mutex& lock)
    : m_lock(lock)
{
    m_window = new sf::RenderWindow(sf::VideoMode(1280, 720), "rendererrr", sf::Style::Default);
    m_rects = new std::vector<sf::RectangleShape>();
}

void IRenderer::loop()
{
    //std::lock_guard<std::mutex> lock(m_lock);
    while (m_window->isOpen())
    {
        sf::Event event;
        while (m_window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                m_window->close();
            }
        }

        render();
    }
}

void IRenderer::render()
{
    //std::lock_guard<std::mutex> lock(m_lock);
    m_window->clear(sf::Color::Black);

    for (size_t i = 0; i < m_rects->size(); i++)
    {
        m_window->draw(m_rects->at(i));
    }

    m_window->display();

}
