#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

struct Player {
    int x, y;
    bool flip = false;
    SDL_Texture* texture = nullptr;
    
    // thong so animation
    int frameWidth = 26;
    int frameHeight = 26;
    int totalFrames = 5;
    int currentFrame = 0;     //khung hinh hien tai
    Uint32 lastFrameTime = 0;
    Uint32 frameDuration = 100; //100ms = 10fps
    bool isAnimating = true;
    
    void move(int dx, int dy);
    void updateAnimation();
    void render(SDL_Renderer* renderer, int offsetX, int offsetY, int verticalPadding);
};

#endif // PLAYER_H