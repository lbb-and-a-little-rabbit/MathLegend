#include "Game.h"

std::vector<char> Game::fontData;
std::vector<char> Game::bgmData;

void Game::LoadTextures(){
    fontData=LoadFile("assets/Font/uifont.ttf");
    bgmData=LoadFile("assets/Music/mainbgm.wav");
}

Game::Game(sf::RenderWindow &window) : window(window){
    //Camera
    camera=window.getDefaultView();
    window.setView(camera);
    
    //Music
    if(!music.openFromMemory(bgmData.data(),bgmData.size())){
        std::cerr << "Failed to load music\n";
        exit(-1);
    }
    else{
        music.setLooping(true);
        music.play();
    }

    //Text
    if(!uiFont.openFromMemory(fontData.data(),fontData.size())){
        std::cerr << "Font load failed\n";
        exit(-1);
    }

    //Map
    map.setSeed(time(nullptr));
    map.generate();
}

GameResult Game::run(){
    GameResult result=GameResult::None;

    //游戏主循环
    while(window.isOpen() && result==GameResult::None){
        processEvents(result);
        update();
        render();
    }

    return result;
}

void Game::processEvents(GameResult &result){
    float dt=delta.restart().asSeconds();
    while(auto event=window.pollEvent()){
        //关闭事件
        if(event->is<sf::Event::Closed>()){
            result=GameResult::Exit;
            return;
        }
        /////////////////////////////////////////
        //按键事件
        if(event->is<sf::Event::KeyPressed>()){
            auto key=event->getIf<sf::Event::KeyPressed>();
            if(key->code==sf::Keyboard::Key::Escape){
                result=GameResult::BackToMenu;
                return;
            }
        }
        /////////////////////////////////////////

    }
    //玩家状态处理更新
    player.handleInput_and_update(dt);
}

void Game::update(){
    camera.setCenter(player.sprite.getPosition());
}

void Game::render(){
    window.clear();

    window.setView(camera);

    map.draw(window,window.getView());
    window.draw(player.sprite);
    //test
    window.draw(player.hitbox);
    window.draw(player.attackBox);

    window.display();
}