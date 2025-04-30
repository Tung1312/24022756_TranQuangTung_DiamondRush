#include "../include/Constants.h"
#include "../include/GameTypes.h"
#include "../include/Player.h"
#include "../include/Renderer.h"
#include "../include/Game.h"
#include "../include/Objects.h"
#include <SDL2/SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {
    if (!init()) {
        std::cout << "Failed to initialize!" << std::endl;
        return -1;
    }
    
    SDL_Ev./ent e;
    bool running = true;
    Uint32 frameStart;
    int frameTime;
    
    while (running) {
        frameStart = SDL_GetTicks();
        
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            } else {
                handleInput(e);
            }
        }
        
        //render game
        render();
        
        //fps
        frameTime = SDL_GetTicks() - frameStart;
        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }
    
    cleanup();
    return 0;
}