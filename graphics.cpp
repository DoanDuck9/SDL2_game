#include "defs.h"
#include "graphics.h"
#include <sstream>
#include <SDL_image.h>

using namespace std;

void drawGrid(SDL_Renderer* renderer, vector<SDL_Texture*>& textures, int arr[])
{
    int boardWidth = GRID_SIZE * TILE_SIZE;
    int boardHeight = GRID_SIZE * TILE_SIZE;
    int startX = (SCREEN_WIDTH - boardWidth) / 2;
    int startY = (SCREEN_HEIGHT - boardHeight) / 2;

    for (int i = 0; i <= GRID_SIZE; ++i)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, startX, startY + i * TILE_SIZE, startX + boardWidth, startY + i * TILE_SIZE);
    }

    for (int j = 0; j <= GRID_SIZE; ++j) // Vẽ đường kẻ dọc
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, startX + j * TILE_SIZE, startY, startX + j * TILE_SIZE, startY + boardHeight);
    }

    for (int i = 0; i < GRID_SIZE; ++i)
    {
        for (int j = 0; j < GRID_SIZE; ++j)
        {
            int val = arr[i * GRID_SIZE + j];
            if (val != 0)
            {
                int padding =1;
                SDL_Rect imageRect ={startX + j * TILE_SIZE + padding,startY + i * TILE_SIZE + padding,TILE_SIZE - 2 * padding,TILE_SIZE - 2 * padding};
                SDL_RenderCopy(renderer, textures[val - 1], NULL, &imageRect);

            }
        }
    }

}
void drawButton(SDL_Renderer* renderer, TTF_Font* font, int x, int y, int width, int height, const char* text)
{
    SDL_Rect rect = {x, y, width, height};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
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

void destroy(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, std::vector<SDL_Texture*> textures, SDL_Texture* setting_texture)
{
    for (SDL_Texture* texture : textures)
    {
        SDL_DestroyTexture(texture);
    }
    SDL_DestroyTexture(setting_texture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}
void drawImageButton(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, int width, int height)
{
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}
