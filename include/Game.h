#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>

//khoi tao
bool init();

void cleanup();

void handleInput(SDL_Event& e);

void resetLevel();

#endif // GAME_H