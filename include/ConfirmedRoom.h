#pragma once

#include "Room.h"
#include "TiledMap.h"

class ConfirmedRoom : public Room
{
private:

    TiledMap map;

    std::string tmxPath;

public:

    ConfirmedRoom(const std::string& path);

    void load() override;

    void draw(sf::RenderWindow& window) override;

    void handleCollisions(Player& player,sf::Vector2f oldpos) override;

    sf::Vector2f getSpawnPoint() const override;
};