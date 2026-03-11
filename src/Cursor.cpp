#include "Cursor.h"

std::vector<sf::Texture> Cursor::cursorTs;

void Cursor::LoadTextures(){
    if(cursorTs.empty()){
        for(int i=1;i<=3;i++){
            std::string path="assets/Cursor/Cursor_0"+std::to_string(i)+".png";
            auto data=LoadFile(path);
            sf::Texture tex;
            if(!tex.loadFromMemory(data.data(),data.size())){
                std::cerr << "Failed to load cursor tex!\n";
                exit(-1);
            }
            cursorTs.push_back(std::move(tex));
        }
    }
}

Cursor::Cursor() : sprite(cursorTs[0]) {}

void Cursor::setStatus(CursorStatus s){
    status=s;
    int type=2;
    switch (status){
    case CursorStatus::idle:
        type=0;
        break;
    case CursorStatus::hit:
        type=1;
        break;
    default:
        break;
    }
    sprite.setTexture(cursorTs[type]);
}

void Cursor::update(sf::RenderWindow& window){
    if(isban)
        setStatus(CursorStatus::ban);
    else if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        setStatus(CursorStatus::hit);
    else
        setStatus(CursorStatus::idle);

    sf::Vector2i pixel = sf::Mouse::getPosition(window);
    sf::Vector2f pos = window.mapPixelToCoords(pixel);

    sprite.setPosition({pos.x - 32, pos.y - 32});
}