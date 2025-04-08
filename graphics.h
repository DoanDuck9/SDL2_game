#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>

void drawGrid(SDL_Renderer* renderer, std::vector<SDL_Texture*>& textures, int arr[]);
void drawButton(SDL_Renderer* renderer, TTF_Font* font, int x, int y, int width, int height, const char* text);
void drawText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color color);
SDL_Texture* loadImage(SDL_Renderer* renderer, const char* filePath);
void destroy(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* rules_texture, std::vector<SDL_Texture*> textures);

#endif
