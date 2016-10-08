//
// Created by Anders on 01.10.2016.
//

#ifndef DIVELIGHT_MENU_HPP
#define DIVELIGHT_MENU_HPP

#include <vector>
#include <functional>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "config.hpp"

using namespace sf;

struct BooleanAction {
    bool& value;
    Keyboard::Key toggle;
    bool active;
};

struct FloatAction {
    float& value;
    float step;
    Keyboard::Key up;
    Keyboard::Key down;
    bool active;
};

struct IntAction {
    int& value;
    int step;
    Keyboard::Key up;
    Keyboard::Key down;
    bool active;
};

template<typename T>
struct EnumAction {
    T& current;
    int min;
    int max;
    Keyboard::Key up;
    Keyboard::Key down;
    bool active;

    EnumAction(T& value, int min, int max,
               Keyboard::Key up, Keyboard::Key down, bool active = false)
        : current(value), min(min), max(max), up(up), down(down), active(active)
    { }

    void increment() {
        int curr = static_cast<T>(current);
        if (curr < max) {
            current = static_cast<T>(current + 1);
        }
    }

    void decrement() {
        int curr = static_cast<T>(current);
        if (curr > min) {
            current = static_cast<T>(current - 1);
        }
    }
};

class Menu {
public:
    Menu(Config& config);

    void poll(sf::Event);
    void render(sf::RenderWindow*);

    void addBooleanAction(bool& value, Keyboard::Key toggle);
    void addFloatAction(float& value, float step, Keyboard::Key up, Keyboard::Key down);
    void addIntAction(int& value, int step, Keyboard::Key up, Keyboard::Key down);

    template<typename T>
    void registerEnumAction(EnumAction<T>* action);

private:
    Config &m_config;

    std::vector<BooleanAction> actionBool;
    std::vector<FloatAction> actionFloat;
    std::vector<IntAction> actionInt;

    EnumAction<ScalingMethod>* actionScaling;
    EnumAction<FreqLimitMethod>* actionFreq;
    EnumAction<WindowFunction>* actionWindow;
    EnumAction<ColorMode>* actionColor;
    EnumAction<FFTSize>* actionSize;
};


#endif //DIVELIGHT_MENU_HPP
