#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>

SDL_Texture* loadTexture(const char* path);

void render();

#endif // RENDERER_H