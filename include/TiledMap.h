#pragma once

#include <SFML/Graphics.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <vector>
#include <string>

class Player;

class TiledMap {

private:

    tmx::Map map;

    std::vector<sf::VertexArray> layers;

    std::vector<sf::Texture> tilesetTextures;

    sf::Vector2f spawnPoint{0,0};

public:

    bool load(const std::string& path);

    void draw(sf::RenderWindow& window);

    void handleCollisions(Player& player, sf::Vector2f oldpos);

    sf::Vector2f getSpawnPoint() const;
};