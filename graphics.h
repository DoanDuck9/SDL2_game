#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>
using namespace std;
void drawGrid(SDL_Renderer* renderer, vector<SDL_Texture*>& textures, int arr[]);
void drawButton(SDL_Renderer* renderer, TTF_Font* font, int x, int y, int width, int height, const char* text);
void drawText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color color);
SDL_Texture* loadImage(SDL_Renderer* renderer, const char* filePath);
void drawSetting(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, int width, int height);
Mix_Chunk* loadMixer(const char* filepath);

#endif
