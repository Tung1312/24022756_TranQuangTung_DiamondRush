#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
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

//menu
extern SDL_Texture* menuBackgroundTexture;
extern SDL_Texture* pressSpaceTexture;
extern Mix_Chunk* menuLoopSound;
extern Mix_Chunk* pressSpaceSound;
extern bool inMenuState;
extern int menuMusicChannel;

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
extern bool wasUnderBoulder;

//debug mode
extern TTF_Font* gameFont;
extern int diamondsCollected;
extern int leavesDestroyed;
extern float currentFPS;
extern bool showDebugOverlay; //debug display toggle

//player death and game over
extern bool isPlayerDead;
extern bool isPlayingDeathSequence;
extern Uint32 playerUnderBoulderStartTime;
extern Uint32 deathSequenceStartTime;
extern SDL_Texture* skeletonTexture;
extern Mix_Chunk* crashSound;
extern Mix_Chunk* gameOverSound;

//game winning condition
extern bool hasWon;
extern std::vector<std::pair<int, int>> victoryTiles;
extern Mix_Chunk* victorySound;

extern int playerStartX;
extern int playerStartY;

#endif // GAME_TYPES_H