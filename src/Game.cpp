#include "../include/Game.h"
#include "../include/Constants.h"
#include "../include/Player.h"
#include "../include/Renderer.h"
#include "../include/Objects.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

SDL_Texture* mapTexture = nullptr;
SDL_Texture* playerTexture = nullptr;
SDL_Texture* leavesTexture = nullptr;
SDL_Texture* boulderTexture = nullptr;
SDL_Texture* diamondTexture = nullptr;

Mix_Chunk* leavesSound = nullptr;
Mix_Chunk* collectSound = nullptr;

//game obj
Player player = {3, 4};
TileList blockedTiles;
TileList leavesTiles;
BlockList diamonds;
BlockList boulderTiles;

bool init() {
    //khoi dong SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cout << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return false;
    }
    
    window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                             SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "Window creation failed: " << SDL_GetError() << std::endl;
        return false;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return false;
    }
    
    //SDL image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << "SDL_image initialization failed: " << IMG_GetError() << std::endl;
        return false;
    }
    
    //mixerrrrrrrrrr
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL_mixer initialization failed: " << Mix_GetError() << std::endl;
        return false;
    }
    
    //load hinh anh
    mapTexture = loadTexture(MAP_PATH);
    playerTexture = loadTexture(PLAYER_PATH);
    leavesTexture = loadTexture(LEAVES_PATH);
    boulderTexture = loadTexture(BOULDER_PATH);
    diamondTexture = loadTexture(DIAMOND_PATH);
    
    // load am thanh
    leavesSound = Mix_LoadWAV(LEAVES_SOUND_PATH);
    collectSound = Mix_LoadWAV(COLLECT_SOUND_PATH);
    
    if (!leavesSound || !collectSound) {
        std::cout << "Failed to load sound effects: " << Mix_GetError() << std::endl;
    }
    
    player.texture = playerTexture;
    
    loadLevelData("levels/map.lvl");
    
    return mapTexture && playerTexture && leavesTexture && boulderTexture && diamondTexture;
}

//input handling
void handleInput(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                player.move(0, -1);
                break;
            case SDLK_DOWN:
                player.move(0, 1);
                break;
            case SDLK_LEFT:
                player.move(-1, 0);
                break;
            case SDLK_RIGHT:
                player.move(1, 0);
                break;
        }
    }
}

//don dep tai nguyen
void cleanup() {
    SDL_DestroyTexture(mapTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(leavesTexture);
    SDL_DestroyTexture(boulderTexture);
    SDL_DestroyTexture(diamondTexture);
    
    Mix_FreeChunk(leavesSound);
    Mix_FreeChunk(collectSound);
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
}