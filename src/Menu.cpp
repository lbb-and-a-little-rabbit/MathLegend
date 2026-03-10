#include "Menu.h"

std::vector<sf::Texture> Menu::menuT;
std::vector<char> Menu::fontData;
std::vector<char> Menu::musicData;
sf::Font Menu::font;

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

    fontData=LoadFile("assets/Font/uifont.ttf");
    if(!font.openFromMemory(fontData.data(),fontData.size())){
        std::cerr << "Failed to load font!";
        exit(-1);
    }

    musicData=LoadFile("assets/Music/menubgm.wav");
}

Menu::Menu(sf::RenderWindow& window) : window(window),sprite(menuT[0]),framecount(0),startText(font),exitText(font) {
    //背景
    sf::Vector2u texSz=menuT[0].getSize();
    sf::Vector2f windowSz=window.getView().getSize();
    sf::Vector2f scale((float)windowSz.x/texSz.x,(float)windowSz.y/texSz.y);
    sprite.setScale(scale);

    //文本
    float wx=window.getView().getSize().x;
    float wy=window.getView().getSize().y;

    startText.setString(L"新游戏");
    startText.setCharacterSize(48);
    startText.setFillColor(sf::Color::White);
    startText.setPosition({wx/2.f, wy/2.f});

    exitText.setString(L"退出");
    exitText.setCharacterSize(48);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition({wx/2.f, wy/2.f+80});

    // 菜单音乐
    if(currentMusic.openFromMemory(musicData.data(),musicData.size())){
        currentMusic.setLooping(true);
        currentMusic.play();
    }
    else{
        std::cerr << "Music loaded failed!";
        exit(-1);
    }

    //Shader
    auto shaderdata=LoadFile("assets/CG/menu.frag");
    std::string shaderStr(shaderdata.begin(), shaderdata.end());
    if(!shader.loadFromMemory(shaderStr, sf::Shader::Type::Fragment)){
        std::cerr << "Shader load failed\n";
        exit(-1);
    }
    screen.setSize(window.getView().getSize());

    //clock首发
    clock.start();
}

MenuResult Menu::run(){
    MenuResult result=MenuResult::None;

    sf::Clock shaderclock;
    while(window.isOpen() && result==MenuResult::None){
        processEvents(result,shaderclock);
        update();
        updateHover();
        render();
    }

    return result;
}

void Menu::processEvents(MenuResult& result,sf::Clock& shaderclock){
    //事件处理
    while(auto event=window.pollEvent()){
        //关闭窗口
        if(event->is<sf::Event::Closed>()){
            result=MenuResult::Exit;
            return;
        }
        ////////////////////////////////////////////////

        //鼠标事件
        if(const auto* mouse=event->getIf<sf::Event::MouseButtonPressed>()){
            if(mouse->button == sf::Mouse::Button::Left){
                // 转换为世界坐标
                sf::Vector2f mousePos = window.mapPixelToCoords(mouse->position);

                // 检查是否点击了 Start Exit 文本
                if(startText.getGlobalBounds().contains(mousePos)){
                    result = MenuResult::StartGame;
                    currentMusic.stop();
                    return; // 点击后立即返回
                }
                if(exitText.getGlobalBounds().contains(mousePos)){
                    result = MenuResult::Exit;
                    return;
                }
            }
        }
        ////////////////////////////////////////////////

        //其他事件

        ////////////////////////////////////////////////
    }
    ////////////////////////////////////////////////

    //shader渲染
    float time = shaderclock.getElapsedTime().asSeconds();

    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    sf::Vector2f mouse = window.mapPixelToCoords(mousePixel);

    shader.setUniform("time", time);
    shader.setUniform("resolution", window.getView().getSize());
    shader.setUniform("mouse", mouse);
    ////////////////////////////////////////////////
}

void Menu::update(){
    if(clock.getElapsedTime().asSeconds()>SwitchTime){
        framecount=(framecount+1)%menuT.size();
        sprite.setTexture(menuT[framecount]);
        clock.restart();
    }
}

void Menu::updateHover() {
    sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if(startText.getGlobalBounds().contains(mouse)) startText.setFillColor(sf::Color::Yellow);   
    else startText.setFillColor(sf::Color::White);
    
    if(exitText.getGlobalBounds().contains(mouse)) exitText.setFillColor(sf::Color::Red);  
    else exitText.setFillColor(sf::Color::White);
}

void Menu::render(){
    window.clear();

    window.draw(sprite);
    window.draw(startText);
    window.draw(exitText);
    window.draw(screen,&shader);

    window.display();
}