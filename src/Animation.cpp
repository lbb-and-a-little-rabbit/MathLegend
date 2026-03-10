#include "Animation.h"

void Animation::update(sf::Sprite &sprite){
    if(clock.getElapsedTime().asSeconds()>speed){
        frame++;
        if(frame>=frameCount){
            if(loop)
                frame = 0;
            else{
                frame = frameCount-1;
                finished = true;
            }
        }
        sprite.setTextureRect(
            sf::IntRect({frame*frameWidth,0},{frameWidth,frameHeight})
        );
        clock.restart();
    }
}

void Animation::reset(){
    frame=0;
    finished=false;
    clock.restart();
}