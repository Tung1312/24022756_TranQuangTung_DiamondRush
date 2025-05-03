#include "../include/Constants.h"
#include "../include/GameTypes.h"
#include "../include/Player.h"
#include "../include/Renderer.h"
#include "../include/Game.h"
#include "../include/Objects.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

// Add these external declarations
extern Mix_Chunk* leavesSound;
extern Mix_Chunk* collectSound;

int main(int argc, char* argv[]) {
    if (!init()) {
        std::cout << "Failed to initialize!" << std::endl;
        return -1;
    }
    
    SDL_Event e;
    bool running = true;
    Uint32 frameStart;
    int frameTime;
    
    //tinh FPS
    Uint32 frameCount = 0;
    Uint32 fpsLastTime = SDL_GetTicks();
    
    while (running) {
        frameStart = SDL_GetTicks();
        
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            } else {
                handleInput(e);
            }
        }
        
        applyGravityToBoulders();
        applyGravityToDiamonds();
        
        //kiem tra xem nguoi choi co dang day boulder khong
        if (player.isPushing) {
            Uint32 currentTime = SDL_GetTicks();
            if (currentTime - player.pushStartTime >= PUSH_DELAY) {
                //thu day boulder
                if (tryPushBoulder(player.x, player.y, player.pushDx, player.pushDy)) {
                    //di chuyen nguoi choi neu thanh cong
                    player.x += player.pushDx;
                    player.y += player.pushDy;
                    
                    //kiem tra leaves voi diamons o vi tri moi
                    for (auto it = leavesTiles.begin(); it != leavesTiles.end(); ++it) {
                        if (it->first == player.x && it->second == player.y) {
                            if (leavesSound) {
                                Mix_PlayChannel(-1, leavesSound, 0);
                            }
                            leavesDestroyed++;
                            leavesTiles.erase(it);
                            break;
                        }
                    }
                    
                    for (auto it = diamonds.begin(); it != diamonds.end(); ++it) {
                        if (it->x == player.x && it->y == player.y) {
                            if (collectSound) {
                                Mix_PlayChannel(-1, collectSound, 0);
                            }
                            diamondsCollected++;
                            diamonds.erase(it);
                            break;
                        }
                    }
                }
                //reset trang thai day boulder
                player.isPushing = false;
            }
        }
        
        //under boulder
        isPlayerUnderBoulder = false;
        for (const auto& boulder : boulderTiles) {
            if (boulder.x == player.x && boulder.y == player.y - 1) {
                isPlayerUnderBoulder = true;
                break;
            }
        }
        
        //update player animation
        player.updateAnimation();
        
        //Render game
        render();
        
        //calculate FPS
        frameCount++;
        if (SDL_GetTicks() - fpsLastTime >= 1000) {
            currentFPS = frameCount * 1000.0f / (SDL_GetTicks() - fpsLastTime);
            frameCount = 0;
            fpsLastTime = SDL_GetTicks();
        }
        
        //fps
        frameTime = SDL_GetTicks() - frameStart;
        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }
    
    cleanup();
    return 0;
}