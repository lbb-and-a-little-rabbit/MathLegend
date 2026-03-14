#include "ConfirmedRoom.h"

ConfirmedRoom::ConfirmedRoom(const std::string& path,float s) : tmxPath(path),scale(s) {}

void ConfirmedRoom::load(){
    map.setScale(scale);
    map.load(tmxPath);
}

void ConfirmedRoom::draw(sf::RenderWindow& window){
    map.draw(window);
}

void ConfirmedRoom::handleCollisions(Player& player,sf::Vector2f oldpos){
    map.handleCollisions(player,oldpos);
}

sf::Vector2f ConfirmedRoom::getSpawnPoint() const{
    return map.getSpawnPoint();
}

void ConfirmedRoom::update(float dt){
    map.update(dt);
}