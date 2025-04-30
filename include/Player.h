#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

struct Player {
    int x, y;
    bool flip = false;
    SDL_Texture* texture = nullptr;
    
    void move(int dx, int dy);
};

#endif // PLAYER_H