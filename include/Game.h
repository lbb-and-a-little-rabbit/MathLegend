#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

#include "Status.h"
#include "Cursor.h"
#include "TrainingRoom.h"

class Game : public Status {
    //Basic
    sf::RenderWindow& window;
    Cursor& cursor;
    sf::Clock delta;

    //Room
    std::unique_ptr<Room> currentRoom;

    //Music
    static std::vector<char> bgmData;
    sf::Music music;

    //Text
    sf::Font uiFont;
    std::vector<sf::Text> helpTexts;

    //Main Elements
    sf::View camera;
    Player player;

    void processEvents(StatusAssemble &result);
    void update();
    void render();

public:
    Game(sf::RenderWindow &window,Cursor &Cursor);
    static void LoadTextures();
    StatusAssemble run() override;
    void changeRoom(std::unique_ptr<Room> newRoom);
};