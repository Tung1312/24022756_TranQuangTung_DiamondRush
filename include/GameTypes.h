#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include <SDL2/SDL.h>
#include <vector>
#include <utility>

struct Player;

struct Block {
    int x, y;
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

#endif // GAME_TYPES_H