#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SDL2/SDL.h>

//debug mode flag
extern bool DEBUG_MODE;

const int SCREEN_WIDTH = 240;
const int SCREEN_HEIGHT = 320;
const int GRID_ROWS = 11;
const int GRID_COLS = 25;
const int TILE_SIZE = 24;
const int GRID_WIDTH = GRID_COLS * TILE_SIZE;
const int GRID_HEIGHT = GRID_ROWS * TILE_SIZE;

const float FALL_SPEED = 1.45f;

const Uint32 PUSH_DELAY = 400;

extern const char* WINDOW_TITLE;
extern const char* MAP_PATH;
extern const char* PLAYER_PATH;
extern const char* LEAVES_PATH;
extern const char* BOULDER_PATH;
extern const char* DIAMOND_PATH;
extern const char* PLAYER_UNDER_BOULDER_PATH;

extern const char* LEAVES_SOUND_PATH;
extern const char* COLLECT_SOUND_PATH;
extern const char* FONT_PATH;

extern const char* LEVEL_PATH;

const int FPS = 60;
const int FRAME_DELAY = 1000 / FPS;
const int FONT_SIZE = 10;

#endif // CONSTANTS_H