#pragma once

#include <SFML/Graphics.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <vector>

#include "PublicResources.h"

class Player;

struct TileAnimation{
    int tilesetIndex;
    int layerIndex; 
    int vertexIndex;

    std::vector<tmx::Tileset::Tile::Animation::Frame> frames;

    float timer = 0.f;
    int currentFrame = 0;
};

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

    void setScale(float s); 

private:
    struct RenderLayer{
        std::vector<sf::VertexArray> vertices; // 每个 tileset 一个
    };

    struct CollisionPolygon {
        std::vector<sf::Vector2f> points;
    };

    tmx::Map map;

    std::vector<sf::Texture> tilesetTextures;
    std::vector<RenderLayer> layers;
    std::vector<TileAnimation> animations;

    std::vector<sf::FloatRect> collisionRects;
    std::vector<CollisionPolygon> collisionPolygons;

    sf::Vector2f spawnPoint;
    float scale=1.f;
};