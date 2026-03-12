#include "ConfirmedRoom.h"

ConfirmedRoom::ConfirmedRoom(const std::string& path) : tmxPath(path) {}

void ConfirmedRoom::load(){
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