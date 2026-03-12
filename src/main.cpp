#include "CGs.h"
#include "StatusMachine.h"
#include "EncryptedPak.h"
#include "Game.h"
#include "Menu.h"
#include <iostream>

int main(int argc,char ** argv){
    //Physfs初始化
    PHYSFS_init(nullptr);
    PHYSFS_mount(".",nullptr,0);
    PHYSFS_setWriteDir(".");

    if(!MountEncryptedPak("data.sxk")){
        std::cerr << "Encryption error!";
        //exit(-1);
    }
    /////////////////////////////////////////////////////////////

    //窗口初始化
    sf::RenderWindow window(sf::VideoMode({1920,1200}),"MathLegend");
    window.setMouseCursorVisible(false);
    sf::View view(sf::FloatRect({0.f,0.f},{(float)window.getSize().x,(float)window.getSize().y}));
    window.setView(view);

    //初期资源加载预备
    CGs::LoadTextures();

    sf::Image icon;
    auto icondata=LoadFile("assets/CG/icon.png");
    if(!icon.loadFromMemory(icondata.data(),icondata.size())){
        std::cerr << "Failed to icon image!\n";
    }
    else window.setIcon(icon);
    /////////////////////////////////////////////////////////////

    //双线程并发，播放CG同时加载游戏资源
    CGs cgs(window);
    std::atomic<bool> assetsLoaded=false;

    std::thread loader(
        [&](){
            //资源加载和初始化
            InitPublicResources();
            Cursor::LoadTextures();
            Menu::LoadTextures();
            Game::LoadTextures();
            House::LoadTextures();
            Tree::LoadTextures();
            TileMap::LoadTextures();
            Block::LoadTextures();
            Player::LoadTextures();
            //加载完毕后设置assetsLoaded为完成，可以终结CG线程
            assetsLoaded=true;
        }
    );
    cgs.run(assetsLoaded);
    loader.join();
    /////////////////////////////////////////////////////////////

    //初始化鼠标光标
    Cursor cursor;

    //双线程均完成后初始化游戏流程状态机，开始进入菜单
    StatusMachine machine;
    machine.changeState(std::make_unique<Menu>(window,cursor));
    /////////////////////////////////////////////////////////////

    //游戏流程状态机循环
    while(window.isOpen()){
        auto* state=machine.get();
        StatusAssemble result=state->run();
        switch(result){
            case StatusAssemble::toGame:
                machine.changeState(std::make_unique<Game>(window,cursor));
                break;
            case StatusAssemble::toMenu:
                machine.changeState(std::make_unique<Menu>(window,cursor));
                break;
            case StatusAssemble::Exit:
                window.close();
                break;
            default:
                break;
        }
    }

    /////////////////////////////////////////////////////////////

    //Physfs关闭
    PHYSFS_deinit();
    /////////////////////////////////////////////////////////////
    return 0;
}