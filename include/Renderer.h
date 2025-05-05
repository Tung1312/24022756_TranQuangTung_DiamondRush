#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <string>

SDL_Texture* loadTexture(const char* path);

void render();

SDL_Texture* renderText(const std::string& text, SDL_Color color);

extern SDL_Texture* guiTexture;

void renderMenu(bool transitioning);

#endif // RENDERER_H