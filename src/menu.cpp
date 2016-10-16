//
// Created by Anders on 01.10.2016.
//

#include "../include/menu.hpp"

Menu::Menu(Config& config)
    : m_config(config)
{
    addBooleanAction(config.applySqrtPostFFT, Keyboard::Key::O);
    addFloatAction(config.linearScalingConstant, 0.5f, Keyboard::Key::Q, Keyboard::Key::A);

    addFloatAction(config.moveDelta, 0.5f, Keyboard::Key::K, Keyboard::Key::L);
    addFloatAction(config.freqLogPower, 0.05f, Keyboard::Key::Z, Keyboard::Key::X);
    addIntAction(config.columnCount, 1, Keyboard::Key::C, Keyboard::Key::V);
    addFloatAction(config.lerpConstant, 0.05f, Keyboard::Key::B, Keyboard::Key::N);
    addFloatAction(config.decayConstant, 0.05f, Keyboard::Key::Comma, Keyboard::Key::Period);

    actionScaling = new EnumAction<ScalingMethod>(config.scalingMethod, 0, 4, sf::Keyboard::Key::W, sf::Keyboard::Key::S, false);
    actionFreq = new EnumAction<FreqLimitMethod>(config.freqLimitMethod, 0, 1, sf::Keyboard::Key::E, sf::Keyboard::Key::D, false);
    actionWindow = new EnumAction<WindowFunction >(config.windowFunction, 0, 3, sf::Keyboard::Key::R, sf::Keyboard::Key::F, false);
    actionSize = new EnumAction<FFTSize>(config.fftSize, 0, 2, sf::Keyboard::Key::T, sf::Keyboard::Key::G, false);
}

void Menu::addBooleanAction(bool& value, Keyboard::Key toggle)
{
    BooleanAction action = { value, toggle, false };
    actionBool.push_back(action);
}

void Menu::addFloatAction(float& value, float step, Keyboard::Key up, Keyboard::Key down)
{
    FloatAction action = { value, step, up, down, false };
    actionFloat.push_back(action);
}

void Menu::addIntAction(int& value, int step, Keyboard::Key up, Keyboard::Key down)
{
    IntAction action = { value, step, up, down, false };
    actionInt.push_back(action);
}

template<typename T>
void Menu::registerEnumAction(EnumAction<T>* action)
{
    if (!action->active) {
        if (Keyboard::isKeyPressed(action->up))
            action->increment();
        else if (Keyboard::isKeyPressed(action->down))
            action->decrement();
        action->active = true;
    } else {
        action->active = false;
    }
}

void Menu::poll(sf::Event event)
{
    registerEnumAction<ScalingMethod>(actionScaling);
    registerEnumAction<FreqLimitMethod>(actionFreq);
    registerEnumAction<WindowFunction>(actionWindow);
    registerEnumAction<FFTSize>(actionSize);
    //registerEnumAction<ColorMode>(actionColor);

    for (auto& action : actionBool) {
        if (Keyboard::isKeyPressed(action.toggle) && !action.active) {
            action.value = !action.value;
            action.active = true;
            std::cout << action.value << std::endl;
        } else {
            action.active = false;
        }
    }

    // TODO: Make templates such that we don't need the same function for different types

    for (auto& action : actionFloat) {
        if (!action.active) {
            if (Keyboard::isKeyPressed(action.up)) {
                action.value += action.step;
                std::cout << action.value << std::endl;
            } else if (Keyboard::isKeyPressed(action.down)) {
                action.value -= action.step;
                std::cout << action.value << std::endl;
            }

            action.active = true;
        } else {
          action.active = false;
        }
    }

    for (auto& action : actionInt) {
        if (!action.active) {
            if (Keyboard::isKeyPressed(action.up)) {
                action.value += action.step;
                std::cout << action.value << std::endl;
            } else if (Keyboard::isKeyPressed(action.down)) {
                action.value -= action.step;
                std::cout << action.value << std::endl;
            }
            action.active = true;
        } else {
            action.active = false;
        }
    }
}

void Menu::render(sf::RenderWindow *target)
{

}