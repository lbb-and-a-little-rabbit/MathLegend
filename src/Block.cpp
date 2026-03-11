#include "Block.h"

sf::Texture Block::tex;

void Block::LoadTextures(){
    auto data=LoadFile("assets/Ground/block.png");
    if(!tex.loadFromMemory(data.data(),data.size())){
        std::cerr << "Failed to load block tex!\n";
        exit(-1);
    }
}

Block::Block(float x,float y) : sprite(tex){
    sprite.setPosition({x,y});
}

void Block::draw(sf::RenderWindow& window){
    window.draw(sprite);
}