#include "House.h"

std::vector<sf::Texture> House::houseTs;

void House::LoadTextures(){
    if(houseTs.empty()){
        for(int i=1;i<=3;i++){
            std::string path="assets/Building/House"+std::to_string(i)+".png";
            auto data=LoadFile(path);
            sf::Texture tex;
            if(!tex.loadFromMemory(data.data(),data.size())){
                std::cerr << "Failed to load tree texture!\n";
                exit(-1);
            }
            houseTs.emplace_back(std::move(tex));
        }
    }
}

House::House(float x,float y): Entity(houseTs[0]) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0,2);  // 均匀分布
    
    type = dis(gen);
    sprite.setTexture(houseTs[type]);
    sprite.setOrigin({96.f,128.f});
    sprite.setPosition({x,y});

    hitbox.setSize({100.f,130.f});
    hitbox.setOrigin({60.f,80.f});
    hitbox.setPosition({x-20.f,y-10.f});
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Blue);
    hitbox.setOutlineThickness(2.f);

}

void House::draw(sf::RenderWindow &window){
    window.draw(sprite);
    window.draw(hitbox);
}

sf::RectangleShape& House::getHitbox(){
    return this->hitbox;
}