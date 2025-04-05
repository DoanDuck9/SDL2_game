#ifndef DEFS_H
#define DEFS_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include <functional>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
extern int stepCount;
extern Uint32 startTime;
extern TTF_Font* font;

class Button {
public:
    std::string text;
    SDL_Rect rect;
    TTF_Font* font;
    SDL_Color color;
    SDL_Texture* texture;
    std::function<void()> onClick;
    Button(const std::string& txt, int x, int y, TTF_Font* f, SDL_Color c, std::function<void()> click);
    void render(SDL_Renderer* renderer);
    void handleEvent(SDL_Event* e);
    void destroy();
};

#endif
