#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <iomanip>
#include <iostream>

#include "TileMap.h"
#include "Animation.h"
#include "physfs_assistant.h"

enum class Forward{
    RIGHT,
    LEFT
};

enum class PlayerStatus{
    idle,
    walk,
    attack,
    hurt,
    death
};

class Player{
public:
    //动画相关
    Animation anim;

    sf::Sprite sprite;
    static sf::Texture idleT;
    static sf::Texture walkT;
    static sf::Texture attackT;
    static sf::Texture hurtT;
    static sf::Texture deathT;

    //状态属性
    PlayerStatus status=PlayerStatus::idle;
    Forward forward=Forward::RIGHT;

    float speed=200.f;
    int value=2;
    bool dead=false;
    bool inVincible=false;
    sf::Clock invclock;

    //攻击相关
    bool lastJ=false;

    //碰撞箱
    sf::RectangleShape hitbox;
    sf::RectangleShape attackBox;

    Player();
    void setStatus(PlayerStatus s);
    void damage();
    sf::Vector2f handleInput_and_update(float dt);
    bool attackHit(const sf::RectangleShape& enemyBox);

    static void LoadTextures();
};