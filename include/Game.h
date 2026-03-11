#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

#include "Cursor.h"
#include "TileMap.h"
#include "Player.h" 

enum class GameResult{
    None,
    BackToMenu,
    Exit,
    Restart
};

class Game{
    //Basic
    sf::RenderWindow& window;
    Cursor& cursor;
    TileMap map;
    sf::Clock delta;

    //Music
    static std::vector<char> bgmData;
    sf::Music music;

    //Text
    static std::vector<char> fontData;
    sf::Font uiFont;
    std::vector<sf::Text> helpTexts;

    //Main Elements
    sf::View camera;
    Player player;

    void processEvents(GameResult &result);
    void update();
    void render();

public:
    Game(sf::RenderWindow &window,Cursor &Cursor);
    static void LoadTextures();
    GameResult run();
};