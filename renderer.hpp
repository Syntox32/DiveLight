
#ifndef DIVELIGHT_RENDERER_H
#define DIVELIGHT_RENDERER_H

#include <vector>
#include <thread>
#include <mutex>
#include <functional>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class IRenderer {
public:
    IRenderer(std::mutex& lock);

    virtual void setData() = 0;
    virtual void render();
    void loop();

protected:
    sf::RenderWindow* m_window;
    std::vector<sf::RectangleShape>* m_rects;
    std::mutex& m_lock;
};


#endif //DIVELIGHT_RENDERER_H
