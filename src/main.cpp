#include "../include/Constants.h"
#include "../include/GameTypes.h"
#include "../include/Player.h"
#include "../include/Renderer.h"
#include "../include/Game.h"
#include "../include/Objects.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

extern Mix_Chunk* leavesSound;
extern Mix_Chunk* collectSound;
extern Mix_Chunk* crashSound;
extern Mix_Chunk* gameOverSound;
extern bool SOUND_ENABLED;

// static bool crashSoundPlayed = false;
// static bool gameOverSoundPlayed = false;

int main(int argc, char* argv[]) {
    if (!init()) {
        std::cout << "Failed to initialize!" << std::endl;
        return -1;
    }
    
    SDL_Event e;
    bool running = true;
    Uint32 frameStart;
    int frameTime;
    
    // //tinh FPS
    // Uint32 frameCount = 0;
    // Uint32 fpsLastTime = SDL_GetTicks();
    
    while (running) {
        frameStart = SDL_GetTicks();

        //victory condition
        if (hasWon) {
            if (!Mix_Playing(-1)) {
                printf("Victory! Level completed.\n");
                running = false;  // exit game loop
            }
            // Only render in victory state, skip game logic
        } 
        else {
            // Process input        
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_r) {
                // R to reset
                if (!isPlayerDead) {
                    isPlayerDead = true;
                    Mix_HaltChannel(-1);
                    // if (gameOverSound && SOUND_ENABLED) {
                    //     Mix_PlayChannel(-1, gameOverSound, 0);
                    // }
                }
            } else {
                handleInput(e);
            }
        }
        
        if (!isPlayerDead) {
            bool wasUnderBoulder = isPlayerUnderBoulder;
            isPlayerUnderBoulder = false;
            
            for (const auto& boulder : boulderTiles) {
                if (boulder.x == player.x && boulder.y == player.y - 1) {
                    isPlayerUnderBoulder = true;
                    break;
                }
            }
            
            //track time under boulder
            if (isPlayerUnderBoulder && !wasUnderBoulder) {
                playerUnderBoulderStartTime = SDL_GetTicks();
            } else if (!isPlayerUnderBoulder) {
                playerUnderBoulderStartTime = 0;
            }
            
            //kiem tra thoi gian duoi boulder 
            if (isPlayerUnderBoulder && 
                SDL_GetTicks() - playerUnderBoulderStartTime >= BOULDER_CRUSH_TIME) {
                
                isPlayerDead = true;
                Mix_HaltChannel(-1);
                
                if (crashSound && SOUND_ENABLED) {
                    Mix_PlayChannel(-1, crashSound, 0);
                }
            }
            
            //game physics
            applyGravityToBoulders();
            applyGravityToDiamonds();
            player.updateAnimation();
        } else {
            //death sequence
            if (!Mix_Playing(-1)) {
                static bool gameOverSoundPlayed = false;
                
                if (!gameOverSoundPlayed) {
                    if (gameOverSound && SOUND_ENABLED) {
                        Mix_PlayChannel(-1, gameOverSound, 0);
                        gameOverSoundPlayed = true;
                    }
                } else {
                    //if game over sound finished, reset level
                    resetLevel();
                    gameOverSoundPlayed = false;
}
                }
            }
        }
        
        render();
        
        //FPS
        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
        
        currentFPS = 1000.0f / (SDL_GetTicks() - frameStart);
    }
    
    cleanup();
    return 0;
}