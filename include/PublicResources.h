#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "physfs_assistant.h"

inline std::vector<char> FontData1;

inline void InitPublicResources(){
    FontData1=LoadFile("assets/Font/uifont.ttf");
}
