#include "../include/Player.h"
#include "../include/Constants.h"
#include "../include/GameTypes.h"
#include "../include/Objects.h"
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <algorithm>

extern Mix_Chunk* leavesSound;
extern Mix_Chunk* collectSound;
extern SDL_Renderer* renderer;

void Player::updateAnimation() {
    if (!isAnimating) return;
    
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastFrameTime > frameDuration) {
        currentFrame = (currentFrame + 1) % totalFrames;
        lastFrameTime = currentTime;
    }
}

void Player::render(SDL_Renderer* renderer, int offsetX, int offsetY, int verticalPadding) {
    if (!texture) {
        std::cout << "Player texture is null" << std::endl;
        return;
    }
    
    // khai bao kich thuoc khung hinh
    SDL_Rect srcRect = {
        currentFrame * frameWidth,
        0,
        frameWidth,
        frameHeight
    };
    
    SDL_Rect destRect = {
        x * TILE_SIZE - offsetX,
        y * TILE_SIZE - offsetY + verticalPadding,
        TILE_SIZE,
        TILE_SIZE
    };
    
    SDL_RenderCopyEx(
        renderer,
        texture,
        &srcRect,
        &destRect,
        0,
        NULL,
        flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE
    );
}

void Player::move(int dx, int dy) {
    //tinh toan vi tri di chuyen
    int newX = x + dx;
    int newY = y + dy;
    
    //lat hinh nguoi choi
    if (dx < 0) {
        flip = true;
        isAnimating = true;
    } else if (dx > 0) {
        flip = false;
        isAnimating = true;
    } else if (dy != 0) {
        isAnimating = true;
    }
    
    //kiem tra gioi han
    if (newX < 0 || newX >= GRID_COLS || newY < 0 || newY >= GRID_ROWS) {
        return;
    }
    
    //kiem tra va cham
    if (isBlockedForPlayer(newX, newY)) {
        return;
    }
    
    // di chuyen den vi tri
    x = newX;
    y = newY;
    
    // kiem tra va cham voi leaves
    // neu co va cham thi xoa tile va play am thanh
    for (auto it = leavesTiles.begin(); it != leavesTiles.end(); ++it) {
        if (it->first == x && it->second == y) {
                        if (leavesSound) {
                Mix_PlayChannel(-1, leavesSound, 0);
            }
            leavesTiles.erase(it);
            break;
        }
    }
    
    //kiem tra va cham voi kim cuong
    // neu co va cham thi xoa tile va play am thanh
    for (auto it = diamonds.begin(); it != diamonds.end(); ++it) {
        if (it->x == x && it->y == y) {
            if (collectSound) {
                Mix_PlayChannel(-1, collectSound, 0);
            }
            diamonds.erase(it);
            break;
        }
    }
}