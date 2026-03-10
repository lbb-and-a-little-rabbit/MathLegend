#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <atomic>
#include <thread>

#include "physfs_assistant.h"

class CGs{
    sf::RenderWindow& window;
    static sf::Font font;
    sf::Text text;

    static std::vector<char> fontdata;
    static std::vector<char> icondata;
    static std::vector<char> logodata;
    static std::vector<char> shaderdata;
    static std::vector<char> musicdata;

public:
    CGs(sf::RenderWindow& window);
    void run(std::atomic<bool>& loaded);

    static void LoadTextures(); // 在 PhysFS 挂载后调用一次
};