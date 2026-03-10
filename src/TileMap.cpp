#include "TileMap.h"

sf::Texture TileMap::stoneT;
sf::Texture TileMap::grassT;
sf::Texture TileMap::soilT;

void TileMap::LoadTextures(){
    auto stonedata=LoadFile("assets/Ground/stone.png");
    auto grassdata=LoadFile("assets/Ground/grass.png");
    auto soildata=LoadFile("assets/Ground/soil.png");
    if(!stoneT.loadFromMemory(stonedata.data(),stonedata.size()) ||
       !grassT.loadFromMemory(grassdata.data(),grassdata.size()) ||
       !soilT.loadFromMemory(soildata.data(),soildata.size())){
           std::cerr << "Failed to load maptiles!\n";
           exit(-1);
    }
}

TileMap::TileMap() : tile(grassT) {
    map.resize(MAP_TILES*MAP_TILES);

    //世界种子
    int permutation[256] ={
        151,160,137,91,90,15,
        131,13,201,95,96,53,194,233,7,225,
        140,36,103,30,69,142,8,99,37,240,
        21,10,23,190,6,148,247,120,234,75,
        0,26,197,62,94,252,219,203,117,35,
        11,32,57,177,33,88,237,149,56,87,
        174,20,125,136,171,168,68,175,74,
        165,71,134,139,48,27,166,77,146,
        158,231,83,111,229,122,60,211,133,
        230,220,105,92,41,55,46,245,40,
        244,102,143,54,65,25,63,161,1,
        216,80,73,209,76,132,187,208,89,
        18,169,200,196,135,130,116,188,
        159,86,164,100,109,198,173,186,
        3,64,52,217,226,250,124,123,5,
        202,38,147,118,126,255,82,85,
        212,207,206,59,227,47,16,58,
        17,182,189,28,42,223,183,170,
        213,119,248,152,2,44,154,163,
        70,221,153,101,155,167,43,
        172,9,129,22,39,253,19,98,
        108,110,79,113,224,232,178,
        185,112,104,218,246,97,228,
        251,34,242,193,238,210,144,
        12,191,179,162,241,81,51,
        145,235,249,14,239,107,49,
        192,214,31,181,199,106,
        157,184,84,204,176,115,
        121,50,45,127,4,150,
        254,138,236,205,93,
        222,114,67,29,24,72,
        243,141,128,195,78,
        66,215,61,156,180
    };

    for(int i=0;i<256;i++){
        p[i]=permutation[i];
        p[i+256]=permutation[i];
    }
}

void TileMap::setSeed(unsigned int seed){
    std::vector<int> perm(256);
    for(int i=0;i<256;i++) perm[i]=i;
    std::mt19937 rng(seed);
    std::shuffle(perm.begin(),perm.end(),rng);
    for(int i=0;i<256;i++){
        p[i]=perm[i];
        p[i+256]=perm[i];
    }
}

float TileMap::fade(float t) const{
    return t*t*t*(t*(t*6-15)+10);
}

float TileMap::lerp(float a,float b,float t) const{
    return a + t*(b-a);
}

float TileMap::grad(int hash,float x,float y) const{
    int h = hash & 7;
    float u = h<4 ? x : y;
    float v = h<4 ? y : x;
    return ((h&1)?-u:u) + ((h&2)?-2*v:2*v);
}

float TileMap::perlin(float x,float y) const{
    int X=(int)floor(x) & 255;
    int Y=(int)floor(y) & 255;//防止超过255

    x-=floor(x);
    y-=floor(y);

    float u = fade(x);
    float v = fade(y);

    int A=p[X]+Y;
    int B=p[X+1]+Y;

    return lerp(
        lerp(grad(p[A],x,y),grad(p[B],x-1,y),u),
        lerp(grad(p[A+1],x,y-1),grad(p[B+1],x-1,y-1),u),
        v
    );
}

void TileMap::generate(){
    float scale=0.03f; //控制生成地形大小

    for(int y=0;y<MAP_TILES;y++){
        for(int x=0;x<MAP_TILES;x++){
            float n=perlin(x*scale,y*scale);

            if(n < -0.15)
                map[y*MAP_TILES+x] = STONE;
            else if(n < 0.15)
                map[y*MAP_TILES+x] = SOIL;
            else
                map[y*MAP_TILES+x] = GRASS;
            }
    }
}

int TileMap::getTile(int x,int y) const{
    return map[y*MAP_TILES+x];
}

void TileMap::draw(sf::RenderWindow& window,const sf::View& camera){
    sf::Vector2f center = camera.getCenter();
    sf::Vector2f size = camera.getSize();

    int startX = (center.x-size.x/2)/TILE_SIZE -2;
    int startY = (center.y-size.y/2)/TILE_SIZE -2;

    int endX = (center.x+size.x/2)/TILE_SIZE +2;
    int endY = (center.y+size.y/2)/TILE_SIZE +2;

    if(startX<0) startX=0;
    if(startY<0) startY=0;

    if(endX>MAP_TILES) endX=MAP_TILES;
    if(endY>MAP_TILES) endY=MAP_TILES;

    for(int y=startY;y<endY;y++){
        for(int x=startX;x<endX;x++){
            int t = map[y*MAP_TILES+x];

            if(t==GRASS) tile.setTexture(grassT);
            if(t==SOIL) tile.setTexture(soilT);
            if(t==STONE) tile.setTexture(stoneT);

            tile.setPosition({(float)x*TILE_SIZE,(float)y*TILE_SIZE});

            window.draw(tile);
        }
    }
}