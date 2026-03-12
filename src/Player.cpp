#include "Player.h"

sf::Texture Player::idleT;
sf::Texture Player::walkT;
sf::Texture Player::attackT;
sf::Texture Player::hurtT;
sf::Texture Player::deathT;

void Player::LoadTextures(){
    const char* files[] = {
        "assets/Player/idle.png",
        "assets/Player/walk.png",
        "assets/Player/attack.png",
        "assets/Player/hurt.png",
        "assets/Player/death.png"
    };
    auto data0=LoadFile(files[0]);
    auto data1=LoadFile(files[1]);
    auto data2=LoadFile(files[2]);
    auto data3=LoadFile(files[3]);
    auto data4=LoadFile(files[4]);
    if( !idleT.loadFromMemory(data0.data(),data0.size()) ||
        !walkT.loadFromMemory(data1.data(),data1.size()) ||
        !attackT.loadFromMemory(data2.data(),data2.size()) ||
        !hurtT.loadFromMemory(data3.data(),data3.size()) ||
        !deathT.loadFromMemory(data4.data(),data4.size())){
            std::cerr << "Failed to load Player's textures!\n";
            exit(-1);
        }
}

Player::Player() : sprite(idleT) {
    sprite.setOrigin({50.f,50.f});
    sprite.setScale({2.f,2.f});
    sprite.setPosition({960.f,600.f});

    sprite.setTextureRect(sf::IntRect({0,0},{100,100}));

    hitbox.setSize({30.f,30.f});
    hitbox.setOrigin({15.f,15.f});
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Red);
    hitbox.setOutlineThickness(2.f);

    attackBox.setSize({30.f,30.f});
    attackBox.setOrigin({15.f,15.f});
    attackBox.setFillColor(sf::Color::Transparent);
    attackBox.setOutlineColor(sf::Color::Blue);
    attackBox.setOutlineThickness(2.f);
}

void Player::setStatus(PlayerStatus s){
    if(status==s) return;
    status=s;

    if(status==PlayerStatus::idle){
        sprite.setTexture(idleT,true);
        anim.frameCount=6;
        anim.loop=true;
    }
    if(status==PlayerStatus::walk){
        sprite.setTexture(walkT,true);
        anim.frameCount=8;
        anim.loop=true;
    }
    if(status==PlayerStatus::attack){
        sprite.setTexture(attackT,true);
        anim.frameCount=6;
        anim.loop=false;
    }
    if(status==PlayerStatus::hurt){
        sprite.setTexture(hurtT,true);
        anim.frameCount=4;
        anim.loop=false;
    }
    if(status==PlayerStatus::death){
        sprite.setTexture(deathT,true);
        anim.frameCount=6;
        anim.loop=false;
    }
    sprite.setTextureRect(sf::IntRect({0,0},{100,100}));
    anim.reset();
}

void Player::damage(){
    if(status==PlayerStatus::death || inVincible) return;
    value--;

    inVincible=true;
    invclock.restart();

    if(value<=0) setStatus(PlayerStatus::death);
    else setStatus(PlayerStatus::hurt);
}

sf::Vector2f Player::handleInput_and_update(float dt){
    sf::Vector2f oldpos=sprite.getPosition();

    if(dead) return oldpos;

    if(inVincible){
        if(invclock.getElapsedTime().asSeconds() > 0.8f)
            inVincible = false;
    }

    PlayerStatus newStatus = status;

    if(status != PlayerStatus::hurt && status != PlayerStatus::death){
        bool nowJ = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J);
        if(nowJ && !lastJ && status != PlayerStatus::attack && status != PlayerStatus::hurt && status != PlayerStatus::death){
            setStatus(PlayerStatus::attack);
        }
        else if(status != PlayerStatus::attack){
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
                newStatus = PlayerStatus::walk;
            else
                newStatus = PlayerStatus::idle;
        }
        lastJ = nowJ;
    }

    if(newStatus != status && status != PlayerStatus::hurt && status != PlayerStatus::death && status != PlayerStatus::attack)
        setStatus(newStatus);

    if(status == PlayerStatus::walk){
        sf::Vector2f move(0,0);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) move.y -= speed*dt;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) move.y += speed*dt;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) move.x -= speed*dt, forward = Forward::LEFT;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) move.x += speed*dt, forward = Forward::RIGHT;

        sf::Vector2f pos = sprite.getPosition() + move;
        sprite.setPosition(pos);
    }

    if(forward == Forward::LEFT)
        sprite.setScale({-2.f,2.f});
    else
        sprite.setScale({2.f,2.f});

    anim.update(sprite);

    if(status == PlayerStatus::attack && anim.finished){
        setStatus(PlayerStatus::idle);
    }

    if(status == PlayerStatus::hurt && anim.finished)
        setStatus(PlayerStatus::idle);

    if(status == PlayerStatus::death && anim.finished)
        dead = true;

    hitbox.setPosition(sprite.getPosition());

    sf::Vector2f pos = sprite.getPosition();

    if(forward == Forward::RIGHT)
        attackBox.setPosition({pos.x + 30,pos.y});
    else
        attackBox.setPosition({pos.x - 30,pos.y});

    return oldpos;
}

bool Player::attackHit(const sf::RectangleShape& enemyBox){
    if(status != PlayerStatus::attack) return false;
    return attackBox.getGlobalBounds().findIntersection(enemyBox.getGlobalBounds()).has_value();
}