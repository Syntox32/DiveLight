
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
    IRenderer();

    virtual void setData(float* data) = 0;
    virtual void beginLoop();
    virtual void render();
    std::mutex m_renderLock;
private:
protected:
    void loop();
    sf::RenderWindow* m_window;
    std::vector<sf::RectangleShape>* m_rects;
    std::thread m_tRender;
};


#endif //DIVELIGHT_RENDERER_H
