#include "TiledMap.h"
#include "Player.h"
#include <iostream>

void TiledMap::setScale(float s){
    scale = s;
}

bool TiledMap::load(const std::string& path){

    if (!map.load(path)){
        std::cout << "TMX load failed\n";
        return false;
    }

    layers.clear();
    tilesetTextures.clear();
    animations.clear();
    collisionRects.clear();
    collisionPolygons.clear();

    spawnPoint = {0.f,0.f};

    const auto& tilesets = map.getTilesets();

    for (auto& ts : tilesets){

        sf::Texture tex;

        if (!tex.loadFromFile(ts.getImagePath())){
            std::cout << "Tileset load failed: "
                      << ts.getImagePath() << "\n";
        }

        tex.setSmooth(false);

        tilesetTextures.push_back(std::move(tex));
    }

    auto mapSize = map.getTileCount();
    auto tileSize = map.getTileSize();

    const auto& mapLayers = map.getLayers();

    for (size_t layerIndex = 0; layerIndex < mapLayers.size(); layerIndex++){

        auto& layer = mapLayers[layerIndex];

        if (layer->getType() != tmx::Layer::Type::Tile)
            continue;

        RenderLayer renderLayer;

        renderLayer.vertices.resize(tilesets.size());

        for (auto& va : renderLayer.vertices)
            va.setPrimitiveType(sf::PrimitiveType::Triangles);

        const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
        const auto& tiles = tileLayer.getTiles();

        for (size_t i = 0; i < tiles.size(); i++){

            uint32_t gid = tiles[i].ID;

            if (!gid) continue;

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

            int tilesPerRow = ts.getColumnCount();

            int tu = localID % tilesPerRow;
            int tv = localID / tilesPerRow;

            int x = i % mapSize.x;
            int y = i / mapSize.x;

            float x0 = x * tileSize.x * scale;
            float y0 = y * tileSize.y * scale;
            float x1 = (x + 1) * tileSize.x * scale;
            float y1 = (y + 1) * tileSize.y * scale;

            float u0 = tu * tileSize.x;
            float v0 = tv * tileSize.y;
            float u1 = u0 + tileSize.x;
            float v1 = v0 + tileSize.y;

            auto& va = renderLayer.vertices[tilesetIndex];

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

            const auto& tsTiles = ts.getTiles();

            for (const auto& t : tsTiles){

                if (t.ID == localID && !t.animation.frames.empty()){

                    TileAnimation anim;

                    anim.tilesetIndex = tilesetIndex;
                    anim.layerIndex = layers.size();
                    anim.vertexIndex = va.getVertexCount() - 6;
                    anim.frames = t.animation.frames;

                    animations.push_back(anim);
                }
            }
        }

        layers.push_back(std::move(renderLayer));
    }

    for (auto& layer : mapLayers){
        if (layer->getType() != tmx::Layer::Type::Object)
            continue;

        auto& objLayer = layer->getLayerAs<tmx::ObjectGroup>();

        if (objLayer.getName() != "collision")
            continue;

        for (auto& obj : objLayer.getObjects())
        {
            auto box = obj.getAABB();

            sf::FloatRect rect(
                {obj.getPosition().x * scale,
                obj.getPosition().y * scale},
                {box.width * scale,
                box.height * scale}
            );

            collisionRects.push_back(rect);
        }
    }

    for (auto& layer : mapLayers){
        if (layer->getType() != tmx::Layer::Type::Object)
            continue;

        auto& objLayer = layer->getLayerAs<tmx::ObjectGroup>();
        if (objLayer.getName() != "collision")
            continue;

        for (auto& obj : objLayer.getObjects()){
            if (obj.getShape() == tmx::Object::Shape::Polygon){
                CollisionPolygon poly;
                for(auto& p : obj.getPoints()){
                    poly.points.push_back({
                        (p.x + obj.getPosition().x) * scale,
                        (p.y + obj.getPosition().y) * scale
                    });
                }
                collisionPolygons.push_back(poly);
            }
        }
    }

    for (auto& layer : mapLayers){
        if (layer->getType() != tmx::Layer::Type::Object)
            continue;

        auto& objLayer = layer->getLayerAs<tmx::ObjectGroup>();
        if (objLayer.getName() != "spawn")
            continue;

        for(auto& obj : objLayer.getObjects()){
            spawnPoint = {
                obj.getPosition().x * scale,
                obj.getPosition().y * scale
            };
        }
    }

    if (spawnPoint == sf::Vector2f(0.f,0.f)){

        spawnPoint = {
            mapSize.x * tileSize.x * scale * 0.5f,
            mapSize.y * tileSize.y * scale * 0.5f
        };
    }

    std::cout
    << "Map loaded\n"
    << "Layers: " << layers.size() << "\n"
    << "Tilesets: " << tilesets.size() << "\n"
    << "Animations: " << animations.size() << "\n";

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

    //test
    if(showhitbox){
        for(auto& rect : collisionRects){
            sf::RectangleShape r;
            r.setPosition(rect.position);
            r.setSize(rect.size);
            r.setFillColor(sf::Color::Transparent);
            r.setOutlineColor(sf::Color::Red);
            r.setOutlineThickness(1);

            window.draw(r);
        }

        for(auto& poly : collisionPolygons){
            sf::VertexArray va(sf::PrimitiveType::LineStrip, poly.points.size() + 1);
            for(size_t i = 0; i < poly.points.size(); ++i){
                va[i].position = poly.points[i];
                va[i].color = sf::Color::Red;
            }
            va[poly.points.size()].position = poly.points[0]; // 闭合
            va[poly.points.size()].color = sf::Color::Red;

            window.draw(va);
        }
    }
    /////////////////////////////////////////////////////
}

void TiledMap::update(float dt){

    const auto& tilesets = map.getTilesets();
    auto tileSize = map.getTileSize();

    for (auto& anim : animations){

        anim.timer += dt;

        auto& frame = anim.frames[anim.currentFrame];

        if (anim.timer >= frame.duration / 1000.f){

            anim.timer = 0.f;

            anim.currentFrame++;

            if (anim.currentFrame >= anim.frames.size())
                anim.currentFrame = 0;

            int newTile =
            anim.frames[anim.currentFrame].tileID;

            const auto& ts =
            tilesets[anim.tilesetIndex];

            int tilesPerRow = ts.getColumnCount();

            int tu = newTile % tilesPerRow;
            int tv = newTile / tilesPerRow;

            float u0 = tu * tileSize.x;
            float v0 = tv * tileSize.y;
            float u1 = u0 + tileSize.x;
            float v1 = v0 + tileSize.y;

            auto& va =
            layers[anim.layerIndex]
            .vertices[anim.tilesetIndex];

            if (anim.vertexIndex + 5 >= va.getVertexCount())
                continue;

            va[anim.vertexIndex + 0].texCoords = {u0,v0};
            va[anim.vertexIndex + 1].texCoords = {u1,v0};
            va[anim.vertexIndex + 2].texCoords = {u1,v1};

            va[anim.vertexIndex + 3].texCoords = {u0,v0};
            va[anim.vertexIndex + 4].texCoords = {u1,v1};
            va[anim.vertexIndex + 5].texCoords = {u0,v1};
        }
    }
}

bool pointInPolygon(const sf::Vector2f& p, const std::vector<sf::Vector2f>& poly){
    bool inside = false;
    for (size_t i = 0, j = poly.size()-1; i < poly.size(); j = i++){
        if (((poly[i].y > p.y) != (poly[j].y > p.y)) &&
            (p.x < (poly[j].x - poly[i].x) * (p.y - poly[i].y) / (poly[j].y - poly[i].y) + poly[i].x)){
            inside = !inside;
        }
    }
    return inside;
}

void TiledMap::handleCollisions(Player& player, sf::Vector2f oldpos){
    sf::FloatRect playerBox = player.hitbox.getGlobalBounds();

    for(auto& rect : collisionRects){
        if(playerBox.findIntersection(rect)){
            player.sprite.setPosition(oldpos);
            break;
        }
    }

    for(auto& poly : collisionPolygons){
        if(pointInPolygon(player.sprite.getPosition(), poly.points)){
            player.sprite.setPosition(oldpos);
            break;
        }
    }

    sf::Vector2f pos = player.sprite.getPosition();

    auto tileSize  = map.getTileSize();
    auto tileCount = map.getTileCount();

    float mapWidth  = tileSize.x * tileCount.x * scale;
    float mapHeight = tileSize.y * tileCount.y * scale;

    float half = 15.f;

    if (pos.x < tileSize.x * scale + half)
        pos.x = tileSize.x * scale + half;

    if (pos.y < tileSize.y * scale + half)
        pos.y = tileSize.y * scale + half;

    if (pos.x > mapWidth - tileSize.x * scale - half)
        pos.x = mapWidth - tileSize.x * scale - half;

    if (pos.y > mapHeight - tileSize.y * scale - half)
        pos.y = mapHeight - tileSize.y * scale - half;

    player.sprite.setPosition(pos);
}

sf::Vector2f TiledMap::getSpawnPoint() const{
    return spawnPoint;
}

sf::Vector2u TiledMap::getTileSize() const{

    auto s = map.getTileSize();

    return {
        static_cast<unsigned>(s.x * scale),
        static_cast<unsigned>(s.y * scale)
    };
}

sf::Vector2u TiledMap::getMapTileCount() const{

    auto s = map.getTileCount();

    return {s.x, s.y};
}

sf::Vector2u TiledMap::getMapSize() const{

    auto tileSize = map.getTileSize();
    auto tileCount = map.getTileCount();

    return {
        static_cast<unsigned>(tileSize.x * tileCount.x * scale),
        static_cast<unsigned>(tileSize.y * tileCount.y * scale)
    };
}