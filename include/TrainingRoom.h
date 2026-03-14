#include "Room.h"
#include "TileMap.h"

class TrainingRoom : public Room {
public:
    TileMap map;

    void load() override {
        map.setSeed(time(nullptr));
        map.generate();
    }

    void draw(sf::RenderWindow& window) override {
        map.draw(window,window.getView());
    }

    void handleCollisions(Player& player,sf::Vector2f oldpos) override {
        map.handleCollisions(player,oldpos);
    }   

    sf::Vector2f getSpawnPoint() const override {
        return {MAP_SIZE/2.f,MAP_SIZE/2.f}; 
    }

    void update(float dt) override {}
};