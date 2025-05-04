#include "../include/Player.h"
#include "../include/Constants.h"
#include "../include/GameTypes.h"
#include "../include/Objects.h"
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <algorithm>

extern Mix_Chunk* leavesSound;
extern Mix_Chunk* collectSound;
extern Mix_Chunk* victorySound;
extern SDL_Renderer* renderer;

Uint32 lastPushTime = 0;

void Player::updateAnimation() {
    //reset pushing state
    if (isPushing) {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - pushStartTime > PUSH_DELAY) {
            isPushing = false;
        }
    }
    
    //animation update
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
    //get current time (for cooldown checks)
    Uint32 currentTime = SDL_GetTicks();
    
    //kiem tra xem nguoi choi co dang day boulder khong
    if (isPushing && currentTime - pushStartTime < PUSH_DELAY) {
        return; //khong cho di chuyen neu dang day boulder
    }
    
    // kiem tra huong di chuyen cua nguoi choi
    // neu nguoi choi di chuyen sang trai va flip = false thi khong di chuyen
    // neu nguoi choi di chuyen sang phai va flip = true thi khong di chuyen
    if ((dx < 0 && !flip) || (dx > 0 && flip)) {
        flip = (dx < 0);
        isAnimating = true;
        return;
    }
    
    //tinh toan vi tri di chuyen
    int newX = x + dx;
    int newY = y + dy;
    
    // update animation
    if (dx != 0 || dy != 0) {
        isAnimating = true;
    }
    
    //kiem tra gioi han
    if (newX < 0 || newX >= GRID_COLS || newY < 0 || newY >= GRID_ROWS) {
        return;
    }
    
    //kiem tra viec day boulder
    for (auto& boulder : boulderTiles) {
        if (boulder.x == newX && boulder.y == newY && !boulder.isFalling) {
            //only push horizontal
            if (dy != 0) return;
            
            // if new push attempt, start pushing timer
            //but no movement
            if (!isPushing) {
                isPushing = true;
                pushStartTime = currentTime;
                pushDx = dx;  //save push direction
                pushDy = dy;
                return;
            }
            
            // if already pushing + the delay passed, complete push
            if (isPushing && currentTime - pushStartTime >= PUSH_DELAY) {
            int pushX = newX + dx;
            int pushY = newY + dy;
            
            // check valid push destination
            if (pushX < 0 || pushX >= GRID_COLS || pushY < 0 || pushY >= GRID_ROWS) {
                return;
            }
            
            // check for walls + other boulders
            if (isBlockedForBlocks(pushX, pushY)) {
                return;
            }
            
            //push success - move boulder
            boulder.x = pushX;
            boulder.y = pushY;
            boulder.pixelX = pushX * TILE_SIZE;
            boulder.pixelY = pushY * TILE_SIZE;
            
            //reset push state
            isPushing = false;
            lastPushTime = currentTime;
            return;
        }

            return;
        }
    }

    // Kiểm tra nếu người chơi đang đẩy boulder nhưng đã di chuyển ra xa
    // (tức là người chơi không còn liền kề với boulder mà họ đang đẩy),
    // hủy trạng thái đẩy để đặt lại tương tác.
    if (isPushing) {
        isPushing = false;
    }
    
    //collision check
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
            leavesDestroyed++;
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
            diamondsCollected++;
            diamonds.erase(it);
            break;
        }
    }
    
    //check victory
    for (const auto& victoryTile : victoryTiles) {
        if (victoryTile.first == x && victoryTile.second == y) {
            if (!hasWon && victorySound && SOUND_ENABLED) {
                Mix_HaltChannel(-1);
                Mix_PlayChannel(-1, victorySound, 0);
                hasWon = true;
            }
            break;
        }
    }
}