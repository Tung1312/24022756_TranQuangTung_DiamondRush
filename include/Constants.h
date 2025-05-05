#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SDL2/SDL.h>

//debug mode flag
extern bool DEBUG_MODE;

//window and grid settings
extern const char* WINDOW_TITLE;
const int SCREEN_WIDTH = 240;
const int SCREEN_HEIGHT = 320;
const int TILE_SIZE = 24;
const int GRID_ROWS = 11;
const int GRID_COLS = 66;
const int GRID_WIDTH = GRID_COLS * TILE_SIZE;
const int GRID_HEIGHT = GRID_ROWS * TILE_SIZE;

//physics
extern float FALL_SPEED;
extern Uint32 PUSH_DELAY;
extern Uint32 BOULDER_CRUSH_TIME;

//assets
extern const char* MENU_BACKGROUND_PATH;
extern const char* PRESS_SPACE_PATH;
extern const char* MAP_PATH;
extern const char* PLAYER_PATH;
extern const char* LEAVES_PATH;
extern const char* BOULDER_PATH;
extern const char* DIAMOND_PATH;
extern const char* PLAYER_UNDER_BOULDER_PATH;
extern const char* SKELETON_PATH;
extern const char* GUI_PATH;

//sounds
extern const char* MENU_LOOP_PATH;
extern const char* PRESS_SPACE_SOUND_PATH;
extern const char* LEAVES_SOUND_PATH;
extern const char* COLLECT_SOUND_PATH;
extern const char* CRASH_SOUND_PATH;
extern const char* GAME_OVER_SOUND_PATH;
extern const char* VICTORY_SOUND_PATH;

//fonts
extern const char* FONT_PATH;

//levels
extern const char* LEVEL_PATH;

//audio settings
extern bool SOUND_ENABLED;
extern bool MUSIC_ENABLED;
extern float MUSIC_VOLUME;
extern float SOUND_VOLUME;

//game settings
const int FPS = 60;
const int FRAME_DELAY = 1000 / FPS;
const int FONT_SIZE = 10;

#endif // CONSTANTS_H