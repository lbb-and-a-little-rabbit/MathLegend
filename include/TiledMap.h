#pragma once

#include <SFML/Graphics.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <vector>

class Player;

class TiledMap{
public:

    bool load(const std::string& path);

    void draw(sf::RenderWindow& window);

    void update(float dt); // 用于动画tile

    void handleCollisions(Player& player, sf::Vector2f oldpos);

    sf::Vector2f getSpawnPoint() const;

    sf::Vector2u getTileSize() const;
    sf::Vector2u getMapTileCount() const;
    sf::Vector2u getMapSize() const;

private:

    struct RenderLayer{
        std::vector<sf::VertexArray> vertices; // 每个 tileset 一个
    };

    tmx::Map map;

    std::vector<sf::Texture> tilesetTextures;
    std::vector<RenderLayer> layers;

    sf::Vector2f spawnPoint;

};