#ifndef DEFS_H
#define DEFS_H

#include <SDL.h>
#include <SDL_ttf.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int TILE_SIZE = 100;

enum GameState {
    STATE_START,
    STATE_PLAYING,
    STATE_SETTINGS,
    STATE_RULES,
    STATE_OVER,
    STATE_LEVEL
};

#endif
