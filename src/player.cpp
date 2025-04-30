#include "../include/Player.h"
#include "../include/Constants.h"
#include <iostream>

void Player::move(int dx, int dy) {
    //simple movement logic
    int newX = x + dx;
    int newY = y + dy;
    
    //kiem tra gioi han di chuyen cua nguoi choi
    if (newX >= 0 && newX < GRID_COLS && newY >= 0 && newY < GRID_ROWS) {
        x = newX;
        y = newY;
        
        //flip state
        if (dx < 0) {
            flip = true;
        } else if (dx > 0) {
            flip = false;
        }
    }
}