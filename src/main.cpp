#include "CGs.h"
#include "Menu.h"
#include "Game.h" 
#include "EncryptedPak.h"
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

    //双线程均完成后进入菜单
MenuLabel:
    Menu menu(window);
    MenuResult result=menu.run();

    if(result==MenuResult::Exit){
        window.close();
    }
    if(result==MenuResult::StartGame){
        Game game(window);
        GameResult gameresult=game.run();
        switch (gameresult){
            case GameResult::Exit:
                window.close();
                break;
            case GameResult::BackToMenu:
                goto MenuLabel;
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