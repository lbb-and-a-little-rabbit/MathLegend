#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "physfs_assistant.h"

enum class CursorStatus{
    idle,
    hit,
    ban
};

class Cursor{
    CursorStatus status=CursorStatus::idle;
    bool isban=false;
public:
    static std::vector<sf::Texture> cursorTs;
    sf::Sprite sprite;
    
    Cursor();
    static void LoadTextures();
    void setStatus(CursorStatus s);
    void update(sf::RenderWindow& window);
};