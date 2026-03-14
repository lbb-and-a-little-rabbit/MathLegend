#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "physfs_assistant.h"

inline std::vector<char> FontData0;
inline std::vector<char> MenuMusicData0;
inline std::vector<char> GameMusicData0;

inline int showhitbox=false;

inline void InitPublicResources(){
    FontData0=LoadFile("assets/Font/uifont.ttf");
    
    MenuMusicData0=LoadFile("assets/Music/menubgm.wav");
    GameMusicData0=LoadFile("assets/Music/mainbgm.wav");
}
