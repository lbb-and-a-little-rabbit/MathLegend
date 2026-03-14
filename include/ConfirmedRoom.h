#pragma once

#include "Room.h"
#include "TiledMap.h"

class ConfirmedRoom : public Room
{
private:

    TiledMap map;

    std::string tmxPath;
    float scale;

public:

    ConfirmedRoom(const std::string& path,float s=1.f);

    void load() override;

    void draw(sf::RenderWindow& window) override;

    void handleCollisions(Player& player,sf::Vector2f oldpos) override;

    sf::Vector2f getSpawnPoint() const override;

    void update(float dt) override;
};