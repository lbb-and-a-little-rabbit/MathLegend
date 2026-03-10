#include "Menu.h"

std::vector<sf::Texture> Menu::menuT;

void Menu::LoadTextures(){
    if(menuT.empty()){
        for(int i=0;i<48;i++){
            std::stringstream ss;
            ss << "assets/Menu/frame_" 
            << std::setw(3) << std::setfill('0') << i  // i 不足三位补0
            << ".png";
            std::string path = ss.str();

            auto data=LoadFile(path);
            sf::Texture tex;
            if(!tex.loadFromMemory(data.data(),data.size())){
                std::cerr << "Failed To Load Menu Textures\n";
                exit(-1);
            }
            menuT.push_back(std::move(tex));
        }
    }
}

Menu::Menu(sf::RenderWindow& window) : window(window),sprite(menuT[0]),framecount(0) {
    sf::Vector2u texSz=menuT[0].getSize();
    sf::Vector2f windowSz=window.getView().getSize();
    sf::Vector2f scale((float)windowSz.x/texSz.x,(float)windowSz.y/texSz.y);
    sprite.setScale(scale);

    clock.start();
}

void Menu::update(){
    if(clock.getElapsedTime().asSeconds()>SwitchTime){
        framecount=(framecount+1)%menuT.size();
        sprite.setTexture(menuT[framecount]);
        clock.restart();
    }
}

MenuResult Menu::run(){
    MenuResult result=MenuResult::None;

    while(window.isOpen() && result==MenuResult::None){
        processEvents(result);
        update();
        render();
    }

    return result;
}

void Menu::processEvents(MenuResult& result){
    while(auto event=window.pollEvent()){
        //关闭窗口
        if(event->is<sf::Event::Closed>()){
            result=MenuResult::Exit;
            return;
        }
        ////////////////////////////////////////////////

        //其他事件
    }
}

void Menu::render(){
    window.clear();

    window.draw(sprite);

    window.display();
}