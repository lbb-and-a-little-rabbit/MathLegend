#include "TiledMap.h"
#include "Player.h"
#include <iostream>

bool TiledMap::load(const std::string& path)
{
    if (!map.load(path)) {
        std::cout << "Failed to load TMX: " << path << "\n";
        return false;
    }

    layers.clear();
    tilesetTextures.clear();

    auto& tilesets = map.getTilesets();

    // 加载 tileset 纹理
    for (auto& ts : tilesets) {
        sf::Texture tex;

        if (!tex.loadFromFile(ts.getImagePath())) {
            std::cout << "Tileset load failed: " << ts.getImagePath() << "\n";
        }

        tilesetTextures.push_back(std::move(tex));
    }

    auto mapSize = map.getTileCount();
    auto tileSize = map.getTileSize();

    const auto& layerList = map.getLayers();

    for (const auto& layer : layerList) {

        if (layer->getType() != tmx::Layer::Type::Tile)
            continue;

        const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();

        sf::VertexArray vertices;
        vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
        vertices.resize(mapSize.x * mapSize.y * 6);

        const auto& tiles = tileLayer.getTiles();

        // tileset 参数
        int spacing = tilesets[0].getSpacing();
        int margin  = tilesets[0].getMargin();

        int texWidth = tilesetTextures[0].getSize().x;

        int tilesPerRow =
        (texWidth - margin * 2 + spacing) /
        (tileSize.x + spacing);

        for (unsigned int i = 0; i < tiles.size(); i++) {

            int tileNumber = tiles[i].ID;

            if (tileNumber == 0)
                continue;

            int tu = (tileNumber - 1) % tilesPerRow;
            int tv = (tileNumber - 1) / tilesPerRow;

            int x = i % mapSize.x;
            int y = i / mapSize.x;

            sf::Vertex* tri = &vertices[i * 6];

            float x0 = x * tileSize.x;
            float y0 = y * tileSize.y;
            float x1 = (x + 1) * tileSize.x;
            float y1 = (y + 1) * tileSize.y;

            float u0 = margin + tu * (tileSize.x + spacing);
            float v0 = margin + tv * (tileSize.y + spacing);
            float u1 = u0 + tileSize.x;
            float v1 = v0 + tileSize.y;

            // triangle 1
            tri[0].position = {x0, y0};
            tri[1].position = {x1, y0};
            tri[2].position = {x1, y1};

            tri[0].texCoords = {u0, v0};
            tri[1].texCoords = {u1, v0};
            tri[2].texCoords = {u1, v1};

            // triangle 2
            tri[3].position = {x0, y0};
            tri[4].position = {x1, y1};
            tri[5].position = {x0, y1};

            tri[3].texCoords = {u0, v0};
            tri[4].texCoords = {u1, v1};
            tri[5].texCoords = {u0, v1};
        }

        layers.push_back(vertices);
    }

    // 查找 spawn 点
    for (const auto& layer : layerList) {

        if (layer->getType() != tmx::Layer::Type::Object)
            continue;

        const auto& objLayer = layer->getLayerAs<tmx::ObjectGroup>();

        for (const auto& obj : objLayer.getObjects()) {

            if (obj.getName() == "spawn") {

                spawnPoint = {
                    obj.getPosition().x,
                    obj.getPosition().y
                };

                break;
            }
        }
    }

    return true;
}

void TiledMap::draw(sf::RenderWindow& window)
{
    for (auto& layer : layers) {

        sf::RenderStates states;

        if (!tilesetTextures.empty())
            states.texture = &tilesetTextures[0];

        window.draw(layer, states);
    }
}

void TiledMap::handleCollisions(Player& player, sf::Vector2f oldpos)
{
    // 暂时留空
}

sf::Vector2f TiledMap::getSpawnPoint() const
{
    return spawnPoint;
}