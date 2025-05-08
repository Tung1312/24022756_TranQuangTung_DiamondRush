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

int main(int argc, char* argv[]) {
    if (!init()) {
        printf("Failed to initialize!\n");
        return -1;
    }
    
    SDL_Event e;
    bool running = true;
    Uint32 frameStart;
    int frameTime;
    bool transitioning = false;
    
    while (running) {
        frameStart = SDL_GetTicks();

        //event handling
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            } else if (inMenuState && !transitioning && e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
                //menu input
                transitioning = true;
                Mix_HaltChannel(menuMusicChannel);
                if (pressSpaceSound && SOUND_ENABLED) {
                    Mix_PlayChannel(-1, pressSpaceSound, 0);
                }
            } else if (!inMenuState) {
                //reset input
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_r) {
                    if (!isPlayerDead) {
                        isPlayerDead = true;
                        Mix_HaltChannel(-1);
                    }
                } else {
                    handleInput(e);
                }
            }
        }
        
        //render game state
        // menu / gameplay
        if (inMenuState) {
            if (transitioning && !Mix_Playing(-1)) {
                inMenuState = false;
                transitioning = false;
            }
            renderMenu(transitioning);
        } else {
            //game loop
            if (hasWon) {
                // player vitory
                if (!Mix_Playing(-1)) {
                    printf("Victory! Level completed.\n");
                    running = false;
                }
            } else {      
                if (!isPlayerDead) {
                    // player alive
                    bool wasUnderBoulder = isPlayerUnderBoulder;
                    isPlayerUnderBoulder = false;
                    
                    for (const auto& boulder : boulderTiles) {
                        if (boulder.x == player.x && boulder.y == player.y - 1) {
                            isPlayerUnderBoulder = true;
                            break;
                        }
                    }
                    //track time under boulder
                    if (isPlayerUnderBoulder) {
                        if (!wasUnderBoulder) {
                            playerUnderBoulderStartTime = SDL_GetTicks();
                        } else if (SDL_GetTicks() - playerUnderBoulderStartTime >= BOULDER_CRUSH_TIME) {
                            isPlayerDead = true;
                            Mix_HaltChannel(-1);
                            
                            if (crashSound && SOUND_ENABLED) {
                                Mix_PlayChannel(-1, crashSound, 0);
                            }
                        }
                    } else {
                        playerUnderBoulderStartTime = 0;
                    }
                    
                    //game physics
                    applyGravityToBoulders();
                    applyGravityToDiamonds();
                    player.updateAnimation();
                } else {
                    //player's unalived themself :L
                    if (!Mix_Playing(-1)) {
                        static bool gameOverSoundPlayed = false;
                        
                        if (!gameOverSoundPlayed) {
                            if (gameOverSound && SOUND_ENABLED) {
                                Mix_PlayChannel(-1, gameOverSound, 0);
                                gameOverSoundPlayed = true;
                            }
                        } else {
                            //reset level
                            resetLevel();
                            gameOverSoundPlayed = false;
                        }
                    }
                }
            }
            render();
        }
        
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