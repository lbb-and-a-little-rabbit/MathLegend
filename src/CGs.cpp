#include "CGs.h"

sf::Font CGs::font;
std::vector<char> CGs::fontdata;
std::vector<char> CGs::icondata;
std::vector<char> CGs::logodata;
std::vector<char> CGs::shaderdata;
std::vector<char> CGs::musicdata;

void CGs::LoadTextures(){
    fontdata=LoadFile("assets/Font/uifont.ttf");
    if(!font.openFromMemory(fontdata.data(),fontdata.size())){
        std::cerr << "Failed to load!";
        exit(-1);
    }

    icondata=LoadFile("assets/CG/icon.png");
    logodata=LoadFile("assets/CG/logo.png");
    musicdata=LoadFile("assets/CG/boot.mp3");
}

CGs::CGs(sf::RenderWindow& window) : window(window),text(font) {}

void CGs::run(std::atomic<bool>& loaded){
    sf::Texture iconTex, logoTex;
    if (!iconTex.loadFromMemory(icondata.data(),icondata.size()) ||
        !logoTex.loadFromMemory(logodata.data(),logodata.size()))
    {
        std::cerr << "Image load failed\n";
        exit(-1);
    }

    sf::Sprite sprite(iconTex);
    sprite.setOrigin({iconTex.getSize().x / 2.f,
                     iconTex.getSize().y / 2.f});
    sprite.setPosition({(float)window.getPosition().x,(float)window.getPosition().y});

    // ===== 加载字体 =====
    text.setCharacterSize(40);
    text.setFillColor(sf::Color::Magenta);

    // ===== 加载音效 =====
    sf::Music music;
    if (!music.openFromMemory(musicdata.data(),musicdata.size()))
    {
        std::cout << "Music load failed\n";
        exit(-1);
    }
    music.play();

    // ===== Shader =====
    auto shaderdata=LoadFile("assets/CG/boot.frag");
    std::string shaderStr(shaderdata.begin(), shaderdata.end());
    sf::Shader shader;
    if (!shader.loadFromMemory(shaderStr,
                             sf::Shader::Type::Fragment))
    {
        std::cout << "Shader load failed\n";
        exit(-1);
    }

    sf::Clock clock;

    while (window.isOpen()){
        if(loaded && clock.getElapsedTime().asSeconds() > 6.f) return;

        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()) {
                window.close();
                exit(0);
            }
        }

        float time = clock.getElapsedTime().asSeconds();
        shader.setUniform("u_time", time);

        // ===== 时间轴切换 =====
        if (time < 3.0f){
            sprite.setTexture(iconTex, true);

            // 第一幕居中
            sprite.setOrigin({iconTex.getSize().x / 2.f,
                            iconTex.getSize().y / 2.f});
            sprite.setPosition({(float)window.getView().getSize().x/2, (float)window.getView().getSize().y/2});

            text.setString("Made With SFML");
            text.setPosition({(float)window.getView().getSize().x/1.5f, (float)window.getView().getSize().y/1.5f});
        }
        else {
            sprite.setTexture(logoTex, true);

            // 第二幕向左上移动
            sprite.setOrigin({logoTex.getSize().x / 2.f,
                            logoTex.getSize().y / 2.f});

            sprite.setPosition({(float)window.getView().getSize().x/2, (float)window.getView().getSize().y/2});

            text.setString(L"LBB 游戏");
            text.setPosition({(float)window.getView().getSize().x/1.5f, (float)window.getView().getSize().y/1.5f});
        }

        // ===== 冲击放大 =====
        float pulse = 0.7f + 0.1f * sin(time * 3.0f);
        sprite.setScale({pulse, pulse});

        window.clear(sf::Color::White);
        window.draw(sprite, &shader);
        window.draw(text);
        window.display();
    }
}