#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "defs.h"

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern TTF_Font* font;

void init();
void close();
void renderMenu();
void renderGame();

#endif
