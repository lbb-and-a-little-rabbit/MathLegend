#pragma once

#include <SFML/Graphics.hpp>

class Player;

class Room{
public:
    virtual ~Room()=default;
    virtual void load()=0;
    virtual void draw(sf::RenderWindow &window)=0;
    virtual void handleCollisions(Player& player,sf::Vector2f oldpos)=0;
    virtual sf::Vector2f getSpawnPoint() const=0;
};