#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <string>

extern SDL_Texture* guiTexture;

SDL_Texture* loadTexture(const char* path);
SDL_Texture* renderText(const std::string& text, SDL_Color color);

void render();
void renderMenu(bool transitioning);

#endif // RENDERER_H