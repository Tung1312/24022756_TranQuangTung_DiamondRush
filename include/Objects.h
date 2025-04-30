#ifndef OBJECTS_H
#define OBJECTS_H

#include "GameTypes.h"
#include <string>

bool isBlockedForPlayer(int x, int y);

void loadLevelData(const std::string& levelFile);

#endif // OBJECTS_H