#ifndef OBJECTS_H
#define OBJECTS_H

#include "GameTypes.h"
#include <string>

//kiem tra xem vi tri co bi chan voi nguoi choi khong
bool isBlockedForPlayer(int x, int y);

//kiem tra xem vi tri co bi chan voi cac block khong
bool isBlockedForBlocks(int x, int y);

//ap dung trong luc cho objs
void applyGravityToBoulders();
void applyGravityToDiamonds();

//kiem tra xem co the day block hay khong
bool tryPushBoulder(int playerX, int playerY, int dx, int dy);

//load map
void loadLevelData(const std::string& levelFile);

#endif // OBJECTS_H