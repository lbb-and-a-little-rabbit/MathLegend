#include "Game.h"

void Game::LoadTextures(){
    
}

Game::Game(sf::RenderWindow &window,Cursor& cursor) : window(window),cursor(cursor){
    //Camera
    camera=window.getDefaultView();
    window.setView(camera);
    
    //Music
    if(!music.openFromMemory(GameMusicData0.data(),GameMusicData0.size())){
        std::cerr << "Failed to load music\n";
        exit(-1);
    }
    else{
        music.setLooping(true);
        music.play();
    }

    //Text
    if(!uiFont.openFromMemory(FontData0.data(),FontData0.size())){
        std::cerr << "Font load failed\n";
        exit(-1);
    }

    //Room
    changeRoom(std::make_unique<ConfirmedRoom>("Tiled/test/mathlegendtest2.tmx"));
}

StatusAssemble Game::run(){
    StatusAssemble result=StatusAssemble::None;

    //游戏主循环
    while(window.isOpen() && result==StatusAssemble::None){
        processEvents(result);
        update();
        render();
    }

    return result;
}

void Game::processEvents(StatusAssemble &result){
    while(auto event=window.pollEvent()){
        //关闭事件
        if(event->is<sf::Event::Closed>()){
            result=StatusAssemble::Exit;
            return;
        }
        /////////////////////////////////////////
        //按键事件
        if(event->is<sf::Event::KeyPressed>()){
            auto key=event->getIf<sf::Event::KeyPressed>();
            if(key->code==sf::Keyboard::Key::Escape){
                result=StatusAssemble::toMenu;
                return;
            }

            ////切换房间测试
            if(key->code==sf::Keyboard::Key::F1){
                changeRoom(std::make_unique<TrainingRoom>());
            }
            /////////////////////////////////////////
        }
        /////////////////////////////////////////
        //其他事件
        cursor.update(window);
    }
}

void Game::update(){
    //玩家状态处理更新
    float dt=delta.restart().asSeconds();
    sf::Vector2f oldpos=player.handleInput_and_update(dt);

    //碰撞检测
    currentRoom->handleCollisions(player,oldpos);

    //摄像头跟随玩家
    camera.setCenter(player.sprite.getPosition());
}

void Game::render(){
    window.clear();

    window.setView(camera);

    currentRoom->draw(window);
    window.draw(player.sprite);
    //test
    window.draw(player.hitbox);
    window.draw(player.attackBox);

    window.draw(cursor.sprite);

    window.display();
}

void Game::changeRoom(std::unique_ptr<Room> newRoom){
    currentRoom=std::move(newRoom);
    currentRoom->load();
    sf::Vector2f spawn=currentRoom->getSpawnPoint();
    player.sprite.setPosition(spawn);
    camera.setCenter(spawn);
}