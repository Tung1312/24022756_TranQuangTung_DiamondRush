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
#include <vector>

bool hasWon = false;
std::vector<std::pair<int, int>> victoryTiles;

bool loadConfigFile(const std::string& filename) {
    std::ifstream configFile(filename);
    if (!configFile.is_open()) {
        printf("Could not open config file: %s\n", filename.c_str());
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
        try {
            if (key == "DEBUG_MODE") {
                DEBUG_MODE = (value == "true" || value == "1");
                printf("Debug mode: %s\n", DEBUG_MODE ? "enabled" : "disabled");
            } 
            else if (key == "WINDOW_TITLE") {
                static std::string windowTitleStr = value;
                WINDOW_TITLE = windowTitleStr.c_str();
                printf("Window title: %s\n", WINDOW_TITLE);
            }
            else if (key == "FALL_SPEED") {
                FALL_SPEED = std::stof(value);
                printf("Fall speed: %.2f\n", FALL_SPEED);
            }
            else if (key == "PUSH_DELAY") {
                PUSH_DELAY = static_cast<Uint32>(std::stoi(value));
                printf("Push delay: %u ms\n", PUSH_DELAY);
            }
            else if (key == "INITIAL_LEVEL") {
                static std::string levelPathStr = value;
                LEVEL_PATH = levelPathStr.c_str();
                printf("Initial level: %s\n", LEVEL_PATH);
            }
            else if (key == "SOUND_ENABLED") {
                SOUND_ENABLED = (value == "true" || value == "1");
                printf("Sound: %s\n", SOUND_ENABLED ? "enabled" : "disabled");
            }
            else if (key == "SOUND_VOLUME") {
                SOUND_VOLUME = std::stof(value);
                if (SOUND_VOLUME < 0.0f) SOUND_VOLUME = 0.0f;
                if (SOUND_VOLUME > 1.0f) SOUND_VOLUME = 1.0f;
                printf("Sound volume: %.2f\n", SOUND_VOLUME);
            }
            else if (key == "MUSIC_ENABLED") {
                MUSIC_ENABLED = (value == "true" || value == "1");
                printf("Music: %s\n", MUSIC_ENABLED ? "enabled" : "disabled");
            }
            else if (key == "MUSIC_VOLUME") {
                MUSIC_VOLUME = std::stof(value);
                if (MUSIC_VOLUME < 0.0f) MUSIC_VOLUME = 0.0f;
                if (MUSIC_VOLUME > 1.0f) MUSIC_VOLUME = 1.0f;
                printf("Music volume: %.2f\n", MUSIC_VOLUME);
            }
        } catch (const std::exception& e) {
            printf("Error parsing config value for %s: %s\n", key.c_str(), e.what());
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
SDL_Texture* guiTexture = nullptr;
SDL_Texture* skeletonTexture = nullptr;

Mix_Chunk* leavesSound = nullptr;
Mix_Chunk* collectSound = nullptr;
Mix_Chunk* crashSound = nullptr;
Mix_Chunk* gameOverSound = nullptr;
Mix_Chunk* victorySound = nullptr;

TTF_Font* gameFont = nullptr;

int diamondsCollected = 0;
int leavesDestroyed = 0;
float currentFPS = 0.0f;

bool showDebugOverlay = false;
bool isPlayerDead = false;
bool isPlayingDeathSequence = false;
bool transitionToSkeletonDone = false;
bool darkenElementsDone = false;
Uint32 playerUnderBoulderStartTime = 0;
Uint32 deathSequenceStartTime = 0;

int playerStartX = 1;
int playerStartY = 1;

//game obj
Player player = {3, 4};
TileList blockedTiles;
TileList leavesTiles;
BlockList diamonds;
BlockList boulderTiles;
bool isPlayerUnderBoulder = false;
bool wasUnderBoulder = false;

bool init() {
    //load config truoc khi khoi dong game
    printf("====================================\n");
    printf("       Loading Configuration        \n");
    printf("====================================\n");
    loadConfigFile("config.cfg");
    
    //khoi dong SDL_main
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return false;
    } else {
        printf("Successfully initialized SDL.\n");
    }
    
    // SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("SDL_image initialization failed: %s\n", IMG_GetError());
        return false;
    } else {
        printf("Successfully initialized SDL_image.\n");
    }
    
    // SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer initialization failed: %s\n", Mix_GetError());
        return false;
    } else {
        printf("Successfully initialized SDL_mixer.\n");
    }
    
    // SDL_ttf
    if (TTF_Init() < 0) {
        printf("SDL_ttf initialization failed: %s\n", TTF_GetError());
        return false;
    } else {
        printf("Successfully initialized SDL_ttf.\n");
    }
    
    //window creation
    window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                             SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        return false;
    }
    
    //create render
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        return false;
    }
        
    //load font
    gameFont = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    if (!gameFont) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return false;
    }
    
    //audio settings
    if (SOUND_ENABLED) {
        Mix_Volume(-1, SOUND_VOLUME * MIX_MAX_VOLUME);
    } else {
        Mix_Volume(-1, 0);
        printf("Sound disabled\n");
    }
    
    if (MUSIC_ENABLED) {
        Mix_VolumeMusic(MUSIC_VOLUME * MIX_MAX_VOLUME);
    } else {
        Mix_VolumeMusic(0);
        printf("Music disabled\n");
    }
    
    //load hinh anh
    printf("====================================\n");
    printf("       Loading Game Elements        \n");
    printf("====================================\n");
    printf("Loading textures...\n");
    printf("Loading map texture (%dx%d)...\n", GRID_WIDTH, GRID_HEIGHT);
    mapTexture = loadTexture(MAP_PATH);
    playerTexture = loadTexture(PLAYER_PATH);
    leavesTexture = loadTexture(LEAVES_PATH);
    boulderTexture = loadTexture(BOULDER_PATH);
    diamondTexture = loadTexture(DIAMOND_PATH);
    playerUnderBoulderTexture = loadTexture(PLAYER_UNDER_BOULDER_PATH); 
    guiTexture = loadTexture(GUI_PATH);
    skeletonTexture = loadTexture(SKELETON_PATH);

    if (!mapTexture || !playerTexture || !leavesTexture || !boulderTexture || !diamondTexture || !playerUnderBoulderTexture || 
        !guiTexture || !skeletonTexture || !leavesSound || !collectSound || !crashSound || !gameOverSound || !victorySound) {
        printf("Failed to load assets: %s\n", Mix_GetError());
    }
    
    // load am thanh
    printf("Loading sound effects...\n");
    leavesSound = Mix_LoadWAV(LEAVES_SOUND_PATH);
    collectSound = Mix_LoadWAV(COLLECT_SOUND_PATH);
    crashSound = Mix_LoadWAV(CRASH_SOUND_PATH);
    gameOverSound = Mix_LoadWAV(GAME_OVER_SOUND_PATH);
    victorySound = Mix_LoadWAV(VICTORY_SOUND_PATH);

    if (!leavesSound || !collectSound || !crashSound || !gameOverSound || !victorySound) {
        printf("Failed to load sound effects: %s\n", Mix_GetError());
    }

    Mix_AllocateChannels(8);
    
    player.texture = playerTexture;
    
    printf("Loading level: %s\n", LEVEL_PATH);
    loadLevelData(LEVEL_PATH);

    printf("====================================\n");
    printf("   Game Initialized Successfully!   \n");
    printf("====================================\n");
    
    return mapTexture && playerTexture && leavesTexture && 
    boulderTexture && diamondTexture && playerUnderBoulderTexture && guiTexture;
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
            case SDLK_F3:
                //toggle if debug mode is enabled
                if (DEBUG_MODE) {
                    showDebugOverlay = !showDebugOverlay;
                }
                break;
        }
    } else if (e.type == SDL_KEYUP) {
    }
}

void resetLevel() {
    //clear sounds
    Mix_HaltChannel(-1);
    
    //reset player position
    player.x = playerStartX;
    player.y = playerStartY;
    player.flip = false;
    player.isAnimating = false;
    player.isPushing = false;
    
    //reset game state
    isPlayerDead = false;
    isPlayerUnderBoulder = false;
    playerUnderBoulderStartTime = 0;
        wasUnderBoulder = false;
    
    // Reset texture colors
    SDL_SetTextureColorMod(mapTexture, 255, 255, 255);
    SDL_SetTextureColorMod(leavesTexture, 255, 255, 255);
    SDL_SetTextureColorMod(diamondTexture, 255, 255, 255);
    SDL_SetTextureColorMod(boulderTexture, 255, 255, 255);
    
    // Clear all game objects
    blockedTiles.clear();
    leavesTiles.clear();
    diamonds.clear();
    boulderTiles.clear();
    
    //reload level data
    loadLevelData(LEVEL_PATH);
    }

//don dep tai nguyen
void cleanup() {
    SDL_DestroyTexture(mapTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(leavesTexture);
    SDL_DestroyTexture(boulderTexture);
    SDL_DestroyTexture(diamondTexture);
    SDL_DestroyTexture(playerUnderBoulderTexture);
    SDL_DestroyTexture(guiTexture);
    SDL_DestroyTexture(skeletonTexture);
    
    Mix_FreeChunk(leavesSound);
    Mix_FreeChunk(collectSound);
    Mix_FreeChunk(crashSound);
    Mix_FreeChunk(gameOverSound);
    Mix_FreeChunk(victorySound);
    
    TTF_CloseFont(gameFont);
    TTF_Quit();
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
}