#include "TiledMap.h"
#include "Player.h"
#include <iostream>

bool TiledMap::load(const std::string& path){
    if (!map.load(path)){
        std::cout << "TMX load failed\n";
        return false;
    }

    layers.clear();
    tilesetTextures.clear();

    const auto& tilesets = map.getTilesets();

    for (auto& ts : tilesets){
        sf::Texture tex;

        if (!tex.loadFromFile(ts.getImagePath())){
            std::cout << "Tileset load failed: "
                      << ts.getImagePath() << "\n";
        }

        tilesetTextures.push_back(std::move(tex));
    }

    auto mapSize = map.getTileCount();
    auto tileSize = map.getTileSize();

    const auto& mapLayers = map.getLayers();

    for (auto& layer : mapLayers){
        if (layer->getType() != tmx::Layer::Type::Tile)
            continue;

        RenderLayer renderLayer;

        renderLayer.vertices.resize(tilesets.size());

        for (auto& va : renderLayer.vertices){
            va.setPrimitiveType(sf::PrimitiveType::Triangles);
        }

        const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
        const auto& tiles = tileLayer.getTiles();

        for (size_t i = 0; i < tiles.size(); i++){
            uint32_t gid = tiles[i].ID;

            if (!gid)
                continue;

            const uint32_t FLIP_H = 0x80000000;
            const uint32_t FLIP_V = 0x40000000;
            const uint32_t FLIP_D = 0x20000000;

            gid &= ~(FLIP_H | FLIP_V | FLIP_D);

            int tilesetIndex = 0;

            for (size_t t = 0; t < tilesets.size(); t++){
                if (gid >= tilesets[t].getFirstGID())
                    tilesetIndex = t;
            }

            const auto& ts = tilesets[tilesetIndex];

            int localID = gid - ts.getFirstGID();

            int spacing = ts.getSpacing();
            int margin = ts.getMargin();

            int texWidth = tilesetTextures[tilesetIndex].getSize().x;

            int tilesPerRow =
                (texWidth - margin * 2 + spacing) /
                (tileSize.x + spacing);

            int tu = localID % tilesPerRow;
            int tv = localID / tilesPerRow;

            int x = i % mapSize.x;
            int y = i / mapSize.x;

            float x0 = x * tileSize.x;
            float y0 = y * tileSize.y;
            float x1 = (x + 1) * tileSize.x;
            float y1 = (y + 1) * tileSize.y;

            float u0 = margin + tu * (tileSize.x + spacing);
            float v0 = margin + tv * (tileSize.y + spacing);
            float u1 = u0 + tileSize.x;
            float v1 = v0 + tileSize.y;

            sf::VertexArray& va = renderLayer.vertices[tilesetIndex];

            sf::Vertex v[6];

            v[0].position = {x0,y0};
            v[1].position = {x1,y0};
            v[2].position = {x1,y1};

            v[3].position = {x0,y0};
            v[4].position = {x1,y1};
            v[5].position = {x0,y1};

            v[0].texCoords = {u0,v0};
            v[1].texCoords = {u1,v0};
            v[2].texCoords = {u1,v1};

            v[3].texCoords = {u0,v0};
            v[4].texCoords = {u1,v1};
            v[5].texCoords = {u0,v1};

            for(int k=0;k<6;k++)
                va.append(v[k]);
        }

        layers.push_back(std::move(renderLayer));
    }

    for (auto& layer : mapLayers){
        if (layer->getType() != tmx::Layer::Type::Object)
            continue;

        auto& objLayer = layer->getLayerAs<tmx::ObjectGroup>();

        for (auto& obj : objLayer.getObjects()){
            if (obj.getName() == "spawn"){
                spawnPoint = {
                    obj.getPosition().x,
                    obj.getPosition().y
                };
            }
        }
    }

    // 如果没有找到 spawn，则设置在地图中心
    if (spawnPoint == sf::Vector2f(0.f, 0.f)){
        auto tileSize = map.getTileSize();
        auto tileCount = map.getTileCount();

        spawnPoint = {
            tileCount.x * tileSize.x * 0.5f,
            tileCount.y * tileSize.y * 0.5f
        };
    }

    return true;
}

void TiledMap::draw(sf::RenderWindow& window){
    for (auto& layer : layers){
        for (size_t i = 0; i < layer.vertices.size(); i++){
            if (layer.vertices[i].getVertexCount() == 0)
                continue;

            sf::RenderStates states;
            states.texture = &tilesetTextures[i];

            window.draw(layer.vertices[i], states);
        }
    }
}

void TiledMap::update(float dt){

}

void TiledMap::handleCollisions(Player& player, sf::Vector2f oldpos){
    //边界处理
    sf::Vector2f pos = player.sprite.getPosition();

    auto tileSize  = map.getTileSize();
    auto tileCount = map.getTileCount();

    float mapWidth  = tileSize.x * tileCount.x;
    float mapHeight = tileSize.y * tileCount.y;

    float half = 15.f;

    if (pos.x < tileSize.x + half)
        pos.x = tileSize.x + half;

    if (pos.y < tileSize.y + half)
        pos.y = tileSize.y + half;

    if (pos.x > mapWidth - tileSize.x - half)
        pos.x = mapWidth - tileSize.x - half;

    if (pos.y > mapHeight - tileSize.y - half)
        pos.y = mapHeight - tileSize.y - half;

    player.sprite.setPosition(pos);
    ///////////////////////////////////////////////////////////////
}

sf::Vector2f TiledMap::getSpawnPoint() const{
    return spawnPoint;
}

sf::Vector2u TiledMap::getTileSize() const{
    auto s = map.getTileSize();
    return {s.x, s.y};
}

sf::Vector2u TiledMap::getMapTileCount() const{
    auto s = map.getTileCount();
    return {s.x, s.y};
}

sf::Vector2u TiledMap::getMapSize() const{
    auto tileSize = map.getTileSize();
    auto tileCount = map.getTileCount();

    return {
        tileSize.x * tileCount.x,
        tileSize.y * tileCount.y
    };
}