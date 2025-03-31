#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "defs.h"
void nhap(int &n);
bool init(SDL_Window*& window, SDL_Renderer*& renderer);
void close(SDL_Window* window, SDL_Renderer* renderer, vector<SDL_Texture*>& numTex);
SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer);
void renderGame(SDL_Renderer* renderer, const vector<int>& board, const vector<SDL_Texture*>& numTex, int n, int tileSize, SDL_Texture* bgTexture,  SDL_Texture* origil);
bool checkWin(const vector<int>& board, int n);
bool inven(const vector<int>& v);
void upimg(vector<SDL_Texture*> &numTex,SDL_Renderer* renderer,int n);
#endif
