#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>

class Animation{
public:
    int frame=0;
    int frameCount=6;
    int frameWidth=100;
    int frameHeight=100;
    float speed=0.12f;
    bool finished=false;
    bool loop=true;

    sf::Clock clock;

    void update(sf::Sprite &sprite);
    void reset();
};