#include "../include/Player.h"
#include "../include/Constants.h"
#include "../include/GameTypes.h"
#include "../include/Objects.h"
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <algorithm>

extern Mix_Chunk* leavesSound;
extern Mix_Chunk* collectSound;

void Player::move(int dx, int dy) {
    //tinh toan vi tri di chuyen
    int newX = x + dx;
    int newY = y + dy;
    
    //lat hinh nguoi choi
    if (dx < 0) {
        flip = true;
    } else if (dx > 0) {
        flip = false;
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