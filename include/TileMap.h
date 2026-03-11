#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm> 
#include <random>
#include <iostream>

#include "physfs_assistant.h"
#include "Block.h"
#include "House.h"

const int TILE_SIZE = 64;
const int MAP_TILES = 100;
const int MAP_SIZE = TILE_SIZE * MAP_TILES;

enum TileType{
    GRASS,
    SOIL,
    STONE
};

class TileMap{
public:

    TileMap();

    void generate();   // 使用柏林噪声生成地图

    void draw(sf::RenderWindow& window,const sf::View& camera);

    int getTile(int x,int y) const;

    void setSeed(unsigned int seed);

    void addEntity(Entity* e);

    void static LoadTextures();

private:

    float perlin(float x,float y) const;
    float fade(float t) const;
    float lerp(float a,float b,float t) const;
    float grad(int hash,float x,float y) const;

private:

    std::vector<int> map;// 缓存友好，速度快

    static sf::Texture grassT;
    static sf::Texture soilT;
    static sf::Texture stoneT;

    sf::Sprite tile;

    //地图上元素
    std::vector<Block> blocks;
    std::vector<Entity*> grid[MAP_TILES][MAP_TILES];

    int p[512]; // permutation table

};