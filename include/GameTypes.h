#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <utility>

struct Player;

//struct block nang cao voi trong luc
struct Block {
    int x, y;                   //grid position
    float pixelX, pixelY;       // actual pixel position for smooth movement
    bool isFalling = false;
    bool needsUpdate = false;   //whether the block needs position update
};

//def nhat kim cuong
typedef std::vector<std::pair<int, int>> TileList;
typedef std::vector<Block> BlockList;

//khai bao game obj
extern Player player;
extern TileList blockedTiles;  //Wall tiles that block movement
extern TileList leavesTiles;   // Destroyable leaves
extern BlockList diamonds;     //Collectible diamonds
extern BlockList boulderTiles; //Movable boulders

//hien thi hinh anh nguoi choi o duoi boulder
extern bool isPlayerUnderBoulder;
extern SDL_Texture* playerUnderBoulderTexture;

//debug mode
extern TTF_Font* gameFont;
extern int diamondsCollected;
extern int leavesDestroyed;
extern float currentFPS;

#endif // GAME_TYPES_H