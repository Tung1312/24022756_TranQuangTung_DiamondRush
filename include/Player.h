#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

struct Player {
    int x, y;
    bool flip = false;
    SDL_Texture* texture = nullptr;
    
    //animation properties
    int frameWidth = 26;      
    int frameHeight = 26;     
    int totalFrames = 5;      
    int currentFrame = 0;     
    Uint32 lastFrameTime = 0; 
    Uint32 frameDuration = 100;
    bool isAnimating = true;  
    
    void move(int dx, int dy);
    void updateAnimation();
    void render(SDL_Renderer* renderer, int offsetX, int offsetY, int verticalPadding);
    
    //flag tracking lieu nguoi choi co dang day block khong
    bool isPushing = false;
    Uint32 pushStartTime = 0;
    int pushDx = 0;
    int pushDy = 0;
};

#endif // PLAYER_H