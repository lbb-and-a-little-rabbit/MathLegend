#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Animation.h"

class Entity{

protected:

    sf::Sprite sprite;
    sf::RectangleShape errorbox;

public:
    Entity(const sf::Texture& tex) : sprite(tex) {}

    virtual ~Entity() = default;

    virtual void update(){}

    virtual void draw(sf::RenderWindow& window){

        window.draw(sprite);

    }

    virtual sf::RectangleShape& getHitbox(){
        std::cerr << "NO hitbox!\n";
        return errorbox;
    }

    sf::Vector2f getPosition() const{

        return sprite.getPosition();

    }

    sf::FloatRect getBounds() const{

        return sprite.getGlobalBounds();

    }

};