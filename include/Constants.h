#ifndef CONSTANTS_H
#define CONSTANTS_H

const int SCREEN_WIDTH = 240;
const int SCREEN_HEIGHT = 320;
const int GRID_ROWS = 11;
const int GRID_COLS = 25;
const int TILE_SIZE = 24;
const int GRID_WIDTH = GRID_COLS * TILE_SIZE;
const int GRID_HEIGHT = GRID_ROWS * TILE_SIZE;

extern const char* WINDOW_TITLE;
extern const char* MAP_PATH;
extern const char* PLAYER_PATH;

const int FPS = 60;
const int FRAME_DELAY = 1000 / FPS;

#endif // CONSTANTS_H