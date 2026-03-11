#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "physfs_assistant.h"

class Block{
    sf::Sprite sprite;
    static sf::Texture tex;

public:
    Block(float x,float y);
    void draw(sf::RenderWindow& window);

    static void LoadTextures();
};