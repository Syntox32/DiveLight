//
// Created by Anders on 01.10.2016.
//

#ifndef DIVELIGHT_MENU_HPP
#define DIVELIGHT_MENU_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "config.hpp"

class Menu {
public:
    Menu(Config& config);

    void poll(sf::Event);
    void render(sf::RenderWindow*);

private:
    Config &m_config;
};


#endif //DIVELIGHT_MENU_HPP
