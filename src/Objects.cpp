#include "../include/Objects.h"
#include "../include/Constants.h"
#include "../include/GameTypes.h"
#include "../include/Player.h"
#include <fstream>
#include <iostream>

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

// load map khi vao game
void loadLevelData(const std::string& levelFile) {
    std::ifstream file(levelFile);
    if (!file) {
        std::cerr << "Failed to open level file: " << levelFile << std::endl;
        return;
    }

    // clear truoc khi load
    blockedTiles.clear();
    leavesTiles.clear();
    diamonds.clear();
    boulderTiles.clear();

    //doc tu lieu tu file .lvl
    //dinh dang file: type x y
    //type: 0 - blocked, 1 - leaves, 2 - diamond, 3 - boulder, 4 - player_start_position
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
                diamonds.push_back({x, y});
                break;
            case 3:
                boulderTiles.push_back({x, y});
                break;
            case 4:
                player.x = x;
                player.y = y;
                break;
        }
    }
    
    file.close();
}