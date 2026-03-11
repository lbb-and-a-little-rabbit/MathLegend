#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <iomanip>
#include <iostream>

#include "physfs_assistant.h"
#include "Cursor.h"

enum class MenuResult{
    None,
    StartGame,
    Exit
};

class Menu{
    //Time
    sf::Clock clock;
    float SwitchTime=0.06f;
    int framecount;

    //Basic
    sf::RenderWindow& window;
    Cursor& cursor;
    static std::vector<char> fontData;
    static std::vector<char> musicData;
    static sf::Font font;
    sf::Text startText;
    sf::Text exitText;

    //Shader
    sf::Shader shader;
    sf::RectangleShape screen;

    //TS
    static std::vector<sf::Texture> menuT;
    sf::Sprite sprite;

    //友元
    friend class Game;

    void processEvents(MenuResult& result,sf::Clock& shaderclock);
    void update();
    void updateHover();
    void render();

public:
    sf::Music currentMusic;

    Menu(sf::RenderWindow& window,Cursor& cursor);
    static void LoadTextures();
    MenuResult run();
};