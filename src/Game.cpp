#include "../include/Game.h"
#include "../include/Constants.h"
#include "../include/Player.h"
#include "../include/Renderer.h"
#include "../include/Objects.h"
#include "../include/GameTypes.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <fstream>
#include <string>

bool loadConfigFile(const std::string& filename) {
    std::ifstream configFile(filename);
    if (!configFile.is_open()) {
        std::cout << "Could not open config file: " << filename << std::endl;
        return false;
    }
    
    std::string line;
    while (std::getline(configFile, line)) {
        //skip comment va empty lines
        if (line.empty() || line[0] == '/' || line[0] == '#')
            continue;
            
        //tim kiem '='
        size_t equalsPos = line.find('=');
        if (equalsPos == std::string::npos)
            continue;
            
        //trich xuat key va value
        std::string key = line.substr(0, equalsPos);
        std::string value = line.substr(equalsPos + 1);
        
        // Trim whitespace from key and value
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);
        
        //key-value pair processing
        if (key == "DEBUG_MODE") {
            if (value == "true" || value == "1") {
                DEBUG_MODE = true;
                std::cout << "Debug mode enabled" << std::endl;
            } else {
                DEBUG_MODE = false;
                std::cout << "Debug mode disabled" << std::endl;
            }
        }
    }
    
    configFile.close();
    return true;
}

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

SDL_Texture* mapTexture = nullptr;
SDL_Texture* playerTexture = nullptr;
SDL_Texture* leavesTexture = nullptr;
SDL_Texture* boulderTexture = nullptr;
SDL_Texture* diamondTexture = nullptr;
SDL_Texture* playerUnderBoulderTexture = nullptr;

Mix_Chunk* leavesSound = nullptr;
Mix_Chunk* collectSound = nullptr;

TTF_Font* gameFont = nullptr;

int diamondsCollected = 0;
int leavesDestroyed = 0;
float currentFPS = 0.0f;

//game obj
Player player = {3, 4};
TileList blockedTiles;
TileList leavesTiles;
BlockList diamonds;
BlockList boulderTiles;
bool isPlayerUnderBoulder = false;

bool init() {
    //load config truoc khi khoi dong game
    loadConfigFile("config.cfg");
    
    //khoi dong SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cout << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Initialize SDL_ttf for text rendering
    if (TTF_Init() < 0) {
        std::cout << "SDL_ttf initialization failed: " << TTF_GetError() << std::endl;
        return false;
    }
    
    // Load font
    gameFont = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    if (!gameFont) {
        std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
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
    playerUnderBoulderTexture = loadTexture(PLAYER_UNDER_BOULDER_PATH); 
    
    // load am thanh
    leavesSound = Mix_LoadWAV(LEAVES_SOUND_PATH);
    collectSound = Mix_LoadWAV(COLLECT_SOUND_PATH);
    
    if (!leavesSound || !collectSound) {
        std::cout << "Failed to load sound effects: " << Mix_GetError() << std::endl;
    }
    
    player.texture = playerTexture;
    
    loadLevelData(LEVEL_PATH);
    
    return mapTexture && playerTexture && leavesTexture && 
    boulderTexture && diamondTexture && playerUnderBoulderTexture;
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
    SDL_DestroyTexture(playerUnderBoulderTexture);
    
    Mix_FreeChunk(leavesSound);
    Mix_FreeChunk(collectSound);
    
    TTF_CloseFont(gameFont);
    TTF_Quit();
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
}