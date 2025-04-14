#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
#include <string>
#include "defs.h"
#include "board.h"
using namespace std;
struct Game
{
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font = nullptr;
    TTF_Font* bigfont = nullptr;
    SDL_Texture* settingTexture = nullptr;
    vector<SDL_Texture*> textures;
    Mix_Chunk* moveSound = nullptr;
    Mix_Chunk* kickSound = nullptr;
    Mix_Chunk* startSound = nullptr;
    Mix_Chunk* winSound = nullptr;
    Mix_Chunk* menuSound = nullptr;

    GameState state = STATE_START;
    int GRID_SIZE=4;
    vector<int> arr;
    //int arr[GRID_SIZE * GRID_SIZE];
    int moves = 0;
    int now = 0;
    int startTime = 0,startSetting=0;

    bool quit = false;

    void init();
    void run();
    void handleEvent(SDL_Event& e);
    void handleStartClick(int x,int y);
    void handlePlayingClick(int x,int y);
    void handleSettingsClick(int x,int y);
    void handleGameOverClick(int x,int y);
    void handleLevelClick(int x,int y);

    void update();
    void render();
    void cleanUp();
};
