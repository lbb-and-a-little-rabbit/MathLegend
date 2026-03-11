#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <ctime>
#include <random>

#include "physfs_assistant.h"
#include "Entity.h"

class Tree : public Entity{
    //类型
    int type=0;

    static std::vector<sf::Texture> treeTs;
    static std::vector<sf::Texture> stumpTs;

public:
    //碰撞箱
    sf::RectangleShape hitbox;

    //动画
    Animation anim;

    Tree(float x,float y);

    void update() override;
    void draw(sf::RenderWindow& window) override;
    sf::RectangleShape& getHitbox() override;
    static void LoadTextures();
};