#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <string>

SDL_Texture* loadTexture(const char* path);

void render();

SDL_Texture* renderText(const std::string& text, SDL_Color color);

#endif // RENDERER_H