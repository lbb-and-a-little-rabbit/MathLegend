#include "Tree.h"

std::vector<sf::Texture> Tree::treeTs;
std::vector<sf::Texture> Tree::stumpTs;

void Tree::LoadTextures(){
    if(treeTs.empty()){
        for(int i=1;i<=4;i++){
            std::string path="assets/Tree/Tree"+std::to_string(i)+".png";
            auto data=LoadFile(path);
            sf::Texture tex;
            if(!tex.loadFromMemory(data.data(),data.size())){
                std::cerr << "Failed to load tree texture!\n";
                exit(-1);
            }
            treeTs.emplace_back(std::move(tex));
        }
    }

    if(stumpTs.empty()){
        for(int i=1;i<=4;i++){
            std::string path="assets/Tree/Stump"+std::to_string(i)+".png";
            auto data=LoadFile(path);
            sf::Texture tex;
            if(!tex.loadFromMemory(data.data(),data.size())){
                std::cerr << "Failed to load tree texture!\n";
                exit(-1);
            }
            stumpTs.emplace_back(std::move(tex));
        }
    }
}

Tree::Tree(float x,float y) : Entity(treeTs[0]) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(2,3);  // 均匀分布
    
    type = dis(gen);
    sprite.setTexture(treeTs[type]);
    float cx=treeTs[type].getSize().x;
    float cy=treeTs[type].getSize().y;
    anim.loop=true;
    anim.frameCount=(int)(cx/cy);
    anim.frameWidth=anim.frameHeight=(int)cy;

    sprite.setPosition({x,y});
    sprite.setTextureRect(sf::IntRect({0,0},{(int)cy,(int)cy}));
    sprite.setOrigin({cy/2.f,cy/2.f});

    hitbox.setSize({60.f,100.f});
    hitbox.setOrigin({20.f,20.f});
    hitbox.setPosition({x-10,y-30.f});
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Green);
    hitbox.setOutlineThickness(2.f);
}

void Tree::update(){
    anim.update(sprite);
}

void Tree::draw(sf::RenderWindow &window){
    window.draw(sprite);
    window.draw(hitbox);
}

sf::RectangleShape& Tree::getHitbox(){
    return hitbox;
}