#include <SDL.h>
#include <SDL_ttf.h>
#include "defs.h"
#include "graphics.h"
#include "board.h"
#include <sstream>
#include <SDL_image.h>
#include <vector>
#include <ctime>
using namespace std;

int main(int argc, char* argv[])
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    GameState currentGameState = STATE_START;
    int moves = 0;
    int startTime = 0;
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);

    window = SDL_CreateWindow("Sliding Puzzle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    font = TTF_OpenFont("fonts/arial.ttf", 28);
    SDL_Texture* rules_texture = loadImage(renderer, "img/huong_dan.png");
    int arr[GRID_SIZE * GRID_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};
    shuffleGrid(arr);
    vector<SDL_Texture*> textures;
    for (int i = 1; i <= 15; ++i)
    {
        string ss="img/num_" + to_string(i) + ".png";
        SDL_Texture* texture = loadImage(renderer, ss.c_str());
        textures.push_back(texture);
    }

    SDL_Event e;
    bool quit = false;

    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int mouse_x, mouse_y;
                SDL_GetMouseState(&mouse_x, &mouse_y);

                if (currentGameState == STATE_START)
                {
                    int buttonWidth = 100;
                    int buttonHeight = 50;
                    int buttonX = (SCREEN_WIDTH - buttonWidth) / 2;
                    int buttonY = (SCREEN_HEIGHT - buttonHeight) / 2;
                    if (mouse_x >= buttonX && mouse_x <= buttonX + buttonWidth && mouse_y >= buttonY && mouse_y <= buttonY + buttonHeight)
                    {
                        currentGameState = STATE_PLAYING;
                        shuffleGrid(arr);
                        while (!isSolvable(arr))
                        {
                            shuffleGrid(arr);
                        }
                        startTime = SDL_GetTicks();
                        moves = 0;
                    }
                }
                else if (currentGameState == STATE_PLAYING)
                {
                    int boardWidth = GRID_SIZE * TILE_SIZE;
                    int boardHeight = GRID_SIZE * TILE_SIZE;
                    int startX = (SCREEN_WIDTH - boardWidth) / 2;
                    int startY = (SCREEN_HEIGHT - boardHeight) / 2;
                    if (mouse_x >= SCREEN_WIDTH - 120 && mouse_x <= SCREEN_WIDTH - 20 && mouse_y >= 10 && mouse_y <= 60)
                    {
                        currentGameState = STATE_SETTINGS;
                    }
                    else if (mouse_x >= startX && mouse_x <= startX + boardWidth && mouse_y >= startY && mouse_y <= startY + boardHeight)
                    {
                        int tile_y = (mouse_x - startX) / TILE_SIZE;
                        int tile_x = (mouse_y - startY) / TILE_SIZE;
                        moveTile(arr, tile_x, tile_y,moves);
                    }
                }
                else if (currentGameState == STATE_SETTINGS)
                {
                    int start_y = (SCREEN_HEIGHT - 170) / 2;
                    if (mouse_x >= (SCREEN_WIDTH - 120) / 2 && mouse_x <= (SCREEN_WIDTH + 120) / 2 && mouse_y >= start_y && mouse_y <= start_y + 50)
                    {
                        currentGameState = STATE_PLAYING;
                    }
                    else if (mouse_x >= (SCREEN_WIDTH - 120) / 2 && mouse_x <= (SCREEN_WIDTH + 120) / 2 && mouse_y >= start_y + 70 && mouse_y <= start_y + 120)
                    {
                        currentGameState = STATE_RULES;
                    }
                    else if (mouse_x >= (SCREEN_WIDTH - 120) / 2 && mouse_x <= (SCREEN_WIDTH + 120) / 2 && mouse_y >= start_y + 140 && mouse_y <= start_y + 190)
                    {
                        quit = true;
                    }
                }
                else if (currentGameState == STATE_RULES)
                {
                    if (mouse_x >= 10 && mouse_x <= 70 && mouse_y >= SCREEN_HEIGHT - 50 && mouse_y <= SCREEN_HEIGHT - 10)
                    {
                        currentGameState = STATE_SETTINGS;
                    }
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        if (currentGameState == STATE_START)
        {
            int buttonWidth = 100;
            int buttonHeight = 50;
            int buttonX = (SCREEN_WIDTH - buttonWidth) / 2;
            int buttonY = (SCREEN_HEIGHT - buttonHeight) / 2;
            drawButton(renderer, font, buttonX, buttonY, buttonWidth, buttonHeight, "Start");
        }
        else if (currentGameState == STATE_PLAYING)
        {
            drawGrid(renderer, textures, arr);
            drawButton(renderer, font, SCREEN_WIDTH - 120, 10, 100, 50, "Setting");
            string s= "";
            s = "Time: " + to_string((SDL_GetTicks() - startTime)/1000)  + "s";
                drawText(renderer, font, s.c_str(), 10, 10, {255, 255, 255});
            s="";
            s = "Moves: " + to_string(moves);
            drawText(renderer, font, s.c_str(), 10, 40, {255, 255, 255});
            if (isGameOver(arr))
            {
                drawText(renderer, font, "You Win!", 200, 200, {255, 255, 255});
                int playAgainButtonWidth = 120;
                int playAgainButtonHeight = 50;
                int playAgainButtonX = (SCREEN_WIDTH - playAgainButtonWidth) / 2;
                int playAgainButtonY = 300;
                drawButton(renderer, font, playAgainButtonX, playAgainButtonY, playAgainButtonWidth, playAgainButtonHeight, "Play Again");
                if (e.type == SDL_MOUSEBUTTONDOWN)
                {
                    int mouse_x, mouse_y;
                    SDL_GetMouseState(&mouse_x, &mouse_y);
                    if (mouse_x >= playAgainButtonX && mouse_x <= playAgainButtonX + playAgainButtonWidth && mouse_y >= playAgainButtonY && mouse_y <= playAgainButtonY + playAgainButtonHeight)
                    {
                        shuffleGrid(arr);
                        while (!isSolvable(arr))
                        {
                            shuffleGrid(arr);
                        }
                        startTime = SDL_GetTicks();
                        moves = 0;
                    }
                }
            }
        }
        else if (currentGameState == STATE_SETTINGS)
        {
            int start_y = (SCREEN_HEIGHT - 170) / 2;
            drawButton(renderer, font, (SCREEN_WIDTH - 120) / 2, start_y, 120, 50, "Continue");
            drawButton(renderer, font, (SCREEN_WIDTH - 120) / 2, start_y + 70, 120, 50, "Rules");
            drawButton(renderer, font, (SCREEN_WIDTH - 120) / 2, start_y + 140, 120, 50, "Exit");
        }
        else if (currentGameState == STATE_RULES)
        {
            SDL_RenderCopy(renderer, rules_texture, NULL, NULL);
            TTF_Font* smallFont = TTF_OpenFont("fonts/arial.ttf", 20);
            drawButton(renderer, smallFont, 10, SCREEN_HEIGHT - 50, 60, 40, "Back");
            TTF_CloseFont(smallFont);
        }

        SDL_RenderPresent(renderer);
    }
    destroy(window, renderer, font, rules_texture, textures);
    return 0;
}
