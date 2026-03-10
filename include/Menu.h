#pragma once


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <iomanip>
#include <iostream>

#include "physfs_assistant.h"

enum MenuResult{
    None,
    StartGame,
    Exit
};

class Menu{
    //Time
    sf::Clock clock;
    float SwitchTime=0.06f;
    int framecount;

    sf::RenderWindow& window;

    //TS
    static std::vector<sf::Texture> menuT;
    sf::Sprite sprite;

    //友元
    friend class Game;

    void processEvents(MenuResult& result);
    void render();

public:
    Menu(sf::RenderWindow& window);
    static void LoadTextures();
    void update();
    MenuResult run();
};