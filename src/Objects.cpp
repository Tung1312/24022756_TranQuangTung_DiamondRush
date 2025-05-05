#include "../include/Objects.h"
#include "../include/Constants.h"
#include "../include/Player.h"
#include "../include/GameTypes.h"
#include <fstream>
#include <iostream>
#include <SDL2/SDL_mixer.h>
extern Mix_Chunk* collectSound;
extern bool SOUND_ENABLED;

// kiem tra di chuyen cua nguoi choi
//neu co vat can tro thi khong di chuyen duoc
bool isBlockedForPlayer(int x, int y) {
    //kiem tra boulders
    for (const auto& boulder : boulderTiles) {
        if (boulder.x == x && boulder.y == y) {
            return true;
        }
    }
    
    //kiem tra walls
    for (const auto& block : blockedTiles) {
        if (block.first == x && block.second == y) {
            return true;
        }
    }
    
    return false;
}

bool isBlockedForBlocks(int x, int y) {
    //walls
    for (const auto& block : blockedTiles) {
        if (block.first == x && block.second == y) {
            return true;
        }
    }
    
    //leaves
    for (const auto& leaf : leavesTiles) {
        if (leaf.first == x && leaf.second == y) {
            return true;
        }
    }
    
    //boulders
    for (const auto& boulder : boulderTiles) {
        if (boulder.x == x && boulder.y == y && !boulder.isFalling) {
            return true;
        }
    }
    
    // diamonds
    for (const auto& diamond : diamonds) {
        if (diamond.x == x && diamond.y == y && !diamond.isFalling) {
            return true;
        }
    }
    
    //player
    if (player.x == x && player.y == y) {
        return true;
    }
    
    // grid boundaries
    if (x < 0 || x >= GRID_COLS || y < 0 || y >= GRID_ROWS) {
        return true;
    }
    
    return false;
}

//ap dung trong luc
void applyGravityToBoulders() {
    
    for (auto& boulder : boulderTiles) {
        //neu boulder khong roi, kiem tra xem co bat dau roi khong
        if (!boulder.isFalling) {
            if (!isBlockedForBlocks(boulder.x, boulder.y + 1)) {
                boulder.isFalling = true;
                //khoi tao vi tri pixel de di chuyen
                boulder.pixelX = boulder.x * TILE_SIZE;
                boulder.pixelY = boulder.y * TILE_SIZE;
                boulder.needsUpdate = true;
            }
        }
        
        //neu boulder dang roi, cap nhat vi tri
        if (boulder.isFalling) {
            boulder.pixelY += FALL_SPEED;
            
            //kiem tra xem boulder da den vi tri moi trong luoi chua
            if (boulder.pixelY >= (boulder.y + 1) * TILE_SIZE) {
                //cap nhat vi tri trong luoi cho boulder
                boulder.y += 1;
                
                //kiem tra xem boulder co tiep tuc roi khong
                if (!isBlockedForBlocks(boulder.x, boulder.y + 1)) {
                    // tiep tuc roi
                    boulder.pixelX = boulder.x * TILE_SIZE;
                    boulder.pixelY = boulder.y * TILE_SIZE;
                } else {
                    // ngung roi
                    boulder.isFalling = false;
                    boulder.needsUpdate = false;
                    boulder.pixelX = boulder.x * TILE_SIZE;
                    boulder.pixelY = boulder.y * TILE_SIZE;

                    //play crash sound
                    if (crashSound && SOUND_ENABLED) {
                        Mix_PlayChannel(-1, crashSound, 0);
                    }
                }
            }
        }
    }
}

// ap dung trong luc cho kim cuong
void applyGravityToDiamonds() {
    for (auto it = diamonds.begin(); it != diamonds.end(); ) {
        bool collected = false;
        
        //kiem tra vi tri cua kim cuong va nguoi choi
        if (it->x == player.x && it->y == player.y && !isPlayerDead) {
            if (collectSound && SOUND_ENABLED) {
                Mix_PlayChannel(-1, collectSound, 0);
            }
            diamondsCollected++;
            collected = true;
        }
        
        if (collected) {
            it = diamonds.erase(it);
            continue;
        }
        
        if (!it->isFalling && !isBlockedForBlocks(it->x, it->y + 1)) {
            it->isFalling = true;
        }
        
        // fall animation handling
        if (it->isFalling) {
            it->pixelY += FALL_SPEED;
            it->needsUpdate = true;
            
            int newGridY = static_cast<int>(it->pixelY / TILE_SIZE);
            
            if (newGridY > it->y) {
                it->y = newGridY;
                
                if (isBlockedForBlocks(it->x, it->y + 1)) {
                    it->isFalling = false;
                    it->pixelY = it->y * TILE_SIZE;
                    it->needsUpdate = false;
                }
            }
        }
        
        ++it;
    }
}

void loadLevelData(const std::string& levelFile) {
    std::ifstream file(levelFile);
    if (!file) {
        printf("Failed to open level file: %s\n", levelFile.c_str());
        return;
    }

    //clear truoc load sau
    blockedTiles.clear();
    leavesTiles.clear();
    diamonds.clear();
    boulderTiles.clear();
    victoryTiles.clear();  // Add this line

    //doc du lieu tu file
    //0: blocked tile, 1: leaves, 2: diamond, 3: boulder, 8: start position, 9: victory
    int type, x, y;
    while (file >> type >> x >> y) {
        switch (type) {
            case 0:
                blockedTiles.emplace_back(x, y);
                break;
            case 1:
                leavesTiles.emplace_back(x, y);
                break;
            case 2:
                {
                    Block diamond = {
                        x, y, 
                        static_cast<float>(x * TILE_SIZE), 
                        static_cast<float>(y * TILE_SIZE), 
                        false, false
                    };
                    diamonds.push_back(diamond);
                }
                break;
            case 3:
                {
                    Block boulder = {
                        x, y, 
                        static_cast<float>(x * TILE_SIZE), 
                        static_cast<float>(y * TILE_SIZE), 
                        false, false
                    };
                    boulderTiles.push_back(boulder);
                }
                break;
            case 8:
                playerStartX = x;
                playerStartY = y;
                player.x = x;
                player.y = y;
                break;
            case 9:
                victoryTiles.emplace_back(x, y);
                break;
        }
    }
    
    file.close();
}

// Thu nghiem viec day boulder
//return true neu block da duoc day thanh cong
bool tryPushBoulder(int playerX, int playerY, int dx, int dy) {
    // Chi cho phep day sang trai (dx < 0) hoac sang phai (dx > 0)
    if (dy != 0) {
        return false; //return false neu di chuyen theo chiu doc
    }
    
    //tinh toan vi tri truoc mat nguoi choi
    int boulderX = playerX + dx;
    int boulderY = playerY + dy;
    
    //tinh toan vi tri xac dinh cua boulder sau khi duoc day
    int targetX = boulderX + dx;
    int targetY = boulderY + dy;
    
    //Ham tim kiem boulder can day
    Block* boulderToPush = nullptr;
    for (auto& boulder : boulderTiles) {
        if (boulder.x == boulderX && boulder.y == boulderY && !boulder.isFalling) {
            boulderToPush = &boulder;
            break;
        }
    }
    
    if (!boulderToPush) {
        return false; //false neu khong tim thay boulder
    }
    
    // kiem tra xem vi tri xac dinh co bi chan khong
    if (isBlockedForBlocks(targetX, targetY)) {
        return false; //false neu bi chan
    }
    
    // cap nhat vi tri cho boulder
    boulderToPush->x = targetX;
    boulderToPush->y = targetY;
    boulderToPush->pixelX = static_cast<float>(targetX * TILE_SIZE);
    boulderToPush->pixelY = static_cast<float>(targetY * TILE_SIZE);
    
    //kiem tra xem boulder co the roi xuong duoi khong
    if (!isBlockedForBlocks(targetX, targetY + 1)) {
        boulderToPush->isFalling = true;
        boulderToPush->needsUpdate = true;
    }
    
    return true; //day boulder thanh cong
}