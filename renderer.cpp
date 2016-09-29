
#include "renderer.hpp"

IRenderer::IRenderer()
{
    m_window = new sf::RenderWindow(sf::VideoMode(1280, 720), "rendererrr", sf::Style::Default);
    m_rects = new std::vector<sf::RectangleShape>();
    m_window->setActive(false);
}

void IRenderer::loop()
{
    m_window->setActive(false);
    while (m_window->isOpen())
    {
        sf::Event event;
        m_window->setActive(true);
        while (m_window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window->close();
            }
        }
        m_window->setActive(false);

        render();
    }
}

void IRenderer::beginLoop()
{
    m_tRender = std::thread(&IRenderer::loop, this);
    //m_tRender.join();
}

void IRenderer::render()
{
    m_window->clear(sf::Color::Red);

    std::lock_guard<std::mutex> lock(m_renderLock);
    for (size_t i = 0; i < m_rects->size(); i++)
    {
        m_window->draw(m_rects->at(i));
    }

    m_window->display();

}
