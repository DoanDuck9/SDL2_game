#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "defs.h"
#include "graphics.h"
#include "board.h"
#include <vector>
#include <string>
#include <sstream>
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

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    window = SDL_CreateWindow("Puzzle Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    font = TTF_OpenFont("fonts/arial.ttf", 24);
    SDL_Texture* setting_texture = loadImage(renderer, "img/setting.png");
    Mix_Chunk* moveSound = Mix_LoadWAV("sound/move.wav");
    Mix_Chunk* kickSound = Mix_LoadWAV("sound/kick.wav");
    Mix_Chunk* startSound = Mix_LoadWAV("sound/start.wav");
    Mix_Chunk* winSound = Mix_LoadWAV("sound/win.wav");
    int arr[GRID_SIZE * GRID_SIZE];
    for (int i = 0; i < GRID_SIZE * GRID_SIZE - 1; ++i)
        arr[i] = i + 1;
    arr[GRID_SIZE * GRID_SIZE - 1] = 0;
    vector<SDL_Texture*> textures;
    for (int i = 1; i <= GRID_SIZE * GRID_SIZE - 1; ++i)
    {
        string ss = "img/" + to_string(i) + ".png";
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
                        if (startSound) Mix_PlayChannel(-1, startSound, 0);
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
                    if (mouse_x >= SCREEN_WIDTH - 40 && mouse_x <= SCREEN_WIDTH - 10 && mouse_y >= 10 && mouse_y <= 40)
                    {
                        if (kickSound) Mix_PlayChannel(-1, kickSound, 0);
                        currentGameState = STATE_SETTINGS;
                    }
                    else if (mouse_x >= startX && mouse_x <= startX + boardWidth && mouse_y >= startY && mouse_y <= startY + boardHeight)
                    {
                        int tile_y = (mouse_x - startX) / TILE_SIZE;
                        int tile_x = (mouse_y - startY) / TILE_SIZE;
                        int check=moves;
                        moveTile(arr, tile_x, tile_y, moves);
                        if (check<moves)
                            Mix_PlayChannel(-1, moveSound, 0);
                    }
                    if(isGameOver(arr))
                        currentGameState=STATE_OVER;
                }
                else if (currentGameState == STATE_SETTINGS)
                {
                    int start_y = (SCREEN_HEIGHT - 170) / 2;
                    if (mouse_x >= (SCREEN_WIDTH - 120) / 2 && mouse_x <= (SCREEN_WIDTH + 120) / 2 && mouse_y >= start_y && mouse_y <= start_y + 50)
                    {
                        if (kickSound) Mix_PlayChannel(-1, kickSound, 0);
                        currentGameState = STATE_PLAYING;
                    }
                    else if (mouse_x >= (SCREEN_WIDTH - 120) / 2 && mouse_x <= (SCREEN_WIDTH + 120) / 2 && mouse_y >= start_y + 60 && mouse_y <= start_y + 190)
                    {
                        if (kickSound) Mix_PlayChannel(-1, kickSound, 0);
                        quit = true;
                    }
                }
                else if(currentGameState== STATE_OVER)
                {
                    int playAgainButtonWidth = 120;
                    int playAgainButtonHeight = 50;
                    int playAgainButtonX = (SCREEN_WIDTH - playAgainButtonWidth) / 2;
                    int playAgainButtonY = 300;
                    if (mouse_x >= playAgainButtonX && mouse_x <= playAgainButtonX + playAgainButtonWidth && mouse_y >= playAgainButtonY && mouse_y <= playAgainButtonY + playAgainButtonHeight)
                    {
                        if (startSound) Mix_PlayChannel(-1, startSound, 0);
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
            }
        }
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
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
            drawSetting(renderer, setting_texture, SCREEN_WIDTH - 40, 10, 30, 30);
            string s = "Time: " + to_string((SDL_GetTicks() - startTime) / 1000) + "s";
            drawText(renderer, font, s.c_str(), 10, 10, {255, 255, 255});
            s = "Moves: " + to_string(moves);
            drawText(renderer, font, s.c_str(), 10, 40, {255, 255, 255});
            if (isGameOver(arr))
            {
                if (winSound) Mix_PlayChannel(-1, winSound, 0);
                SDL_SetRenderDrawColor(renderer,0,0,0,255);
                SDL_RenderClear(renderer);
                TTF_Font* bigfont = TTF_OpenFont("fonts/arial.ttf", 30);
                drawText(renderer, bigfont, "You Win!", 340, 200, {255, 255, 255});
                    TTF_CloseFont(bigfont);

                int playAgainButtonWidth = 120;
                int playAgainButtonHeight = 50;
                int playAgainButtonX = (SCREEN_WIDTH - playAgainButtonWidth) / 2;
                int playAgainButtonY = 300;
                drawButton(renderer, font, playAgainButtonX, playAgainButtonY, playAgainButtonWidth, playAgainButtonHeight, "Play Again");
            }
        }
        else if (currentGameState == STATE_SETTINGS)
        {
            int start_y = (SCREEN_HEIGHT - 170) / 2;
            drawButton(renderer, font, (SCREEN_WIDTH - 120) / 2, start_y, 120, 50, "Continue");
            drawButton(renderer, font, (SCREEN_WIDTH - 120) / 2, start_y + 60, 120, 50, "Exit");
        }
        SDL_RenderPresent(renderer);
    }
    destroy(window, renderer, font, textures,setting_texture);
    Mix_FreeChunk(moveSound);
    Mix_FreeChunk(kickSound);
    Mix_FreeChunk(startSound);
    Mix_FreeChunk(winSound);
    Mix_CloseAudio();
    return 0;
}
