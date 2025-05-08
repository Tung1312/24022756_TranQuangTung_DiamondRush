#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <utility>

//khai bao forward struct player
struct Player;

//khai bao struct block
struct Block {
    int x, y;
    float pixelX, pixelY;
    bool isFalling = false;
    bool needsUpdate = false;
};

//menu
extern SDL_Texture* menuBackgroundTexture;
extern SDL_Texture* pressSpaceTexture;
extern Mix_Chunk* menuLoopSound;
extern Mix_Chunk* pressSpaceSound;
extern bool inMenuState;
extern int menuMusicChannel;

//def block va tile
typedef std::vector<std::pair<int, int>> TileList;
typedef std::vector<Block> BlockList;

//khai bao game obj
extern Player player;
extern TileList blockedTiles;
extern TileList leavesTiles;
extern TileList victoryTiles;
extern BlockList diamonds;
extern BlockList boulderTiles;

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
extern Mix_Chunk* victorySound;

extern int playerStartX;
extern int playerStartY;

#endif // GAME_TYPES_H