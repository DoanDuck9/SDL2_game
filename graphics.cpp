#include "defs.h"
#include "graphics.h"
#include <sstream>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include<vector>
using namespace std;

void drawGrid(SDL_Renderer* renderer, vector<SDL_Texture*>& textures,vector<int> arr,int GRID_SIZE)
{
    int boardWidth = GRID_SIZE * TILE_SIZE;
    int boardHeight = GRID_SIZE * TILE_SIZE;
    int startX = (SCREEN_WIDTH - boardWidth) / 2;
    int startY = (SCREEN_HEIGHT - boardHeight) / 2;

    for (int i = 0; i <= GRID_SIZE; ++i)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_RenderDrawLine(renderer, startX, startY + i * TILE_SIZE, startX + boardWidth, startY + i * TILE_SIZE);
        SDL_RenderDrawLine(renderer, startX, startY + i * TILE_SIZE-1, startX + boardWidth, startY + i * TILE_SIZE-1);
        SDL_RenderDrawLine(renderer, startX, startY + i * TILE_SIZE+1, startX + boardWidth, startY + i * TILE_SIZE+1);
    }

    for (int j = 0; j <= GRID_SIZE; ++j)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, startX + j * TILE_SIZE, startY, startX + j * TILE_SIZE, startY + boardHeight);
        SDL_RenderDrawLine(renderer, startX + j * TILE_SIZE+1, startY, startX + j * TILE_SIZE+1, startY + boardHeight);
        SDL_RenderDrawLine(renderer, startX + j * TILE_SIZE-1, startY, startX + j * TILE_SIZE-1, startY + boardHeight);
    }

    for (int i = 0; i < GRID_SIZE; ++i)
    {
        for (int j = 0; j < GRID_SIZE; ++j)
        {
            int val = arr[i * GRID_SIZE + j];
            if (val != 0)
            {
                int padding = 1;
                SDL_Rect imageRect = {startX + j * TILE_SIZE + padding, startY + i * TILE_SIZE + padding, TILE_SIZE - 2 * padding, TILE_SIZE - 2 * padding};
                SDL_RenderCopy(renderer, textures[val - 1], NULL, &imageRect);
            }
        }
    }
}

void drawButton(SDL_Renderer* renderer, TTF_Font* font, int x, int y, int width, int height, const char* text)
{
    SDL_Rect shadowRect = {x - 4, y - 4, width, height};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100); // Màu đen mờ (alpha = 100)
    SDL_RenderFillRect(renderer, &shadowRect);
    SDL_Rect rect = {x, y, width, height};
//    if (isPressed)
  //      SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); // Màu tối hơn khi nhấn
   // else
        if (isMouseOver(x, y, width, height))
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // Màu hover
    else
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Mặc định
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);

    int textWidth, textHeight;
    TTF_SizeText(font, text, &textWidth, &textHeight);
    drawText(renderer, font, text, x + (width - textWidth) / 2, y + (height - textHeight) / 2, {255, 255, 255});
}
void drawText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color color)
{
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
Mix_Chunk* loadMixer(const char* filepath)
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_Log("SDL_mixer could not initialize! SDL_mixer Error: %s", Mix_GetError());
        return nullptr;
    }

    Mix_Chunk* sound = Mix_LoadWAV(filepath);
    if (!sound)
    {
        SDL_Log("Failed to load sound file (%s)! SDL_mixer Error: %s", filepath, Mix_GetError());
        return nullptr;
    }

    Mix_VolumeChunk(sound, MIX_MAX_VOLUME);
    return sound;
}

SDL_Texture* loadImage(SDL_Renderer* renderer, const char* filePath)
{
    SDL_Surface* surface = IMG_Load(filePath);
    if (surface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", filePath, IMG_GetError());
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void drawSetting(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, int width, int height)
{
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}
bool isMouseOver(int x, int y, int w, int h)
{
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    return (mx >= x && mx <= x + w && my >= y && my <= y + h);
}
