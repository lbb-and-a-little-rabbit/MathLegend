#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <ctime>
#include <random>

#include "physfs_assistant.h"
#include "Entity.h"

class House : public Entity{
    //类型
    int type=0;

    static std::vector<sf::Texture> houseTs;
public:
    //碰撞箱
    sf::RectangleShape hitbox;

    House(float x,float y);

    void draw(sf::RenderWindow& window) override;
    sf::RectangleShape& getHitbox() override;
    static void LoadTextures();
};