#include "Game.h"
#include "graphics.h"
#include "board.h"
#include <ctime>
#include <sstream>
void Game::init()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    window = SDL_CreateWindow("Puzzle Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    font = TTF_OpenFont("fonts/arial.ttf", 24);
    bigfont = TTF_OpenFont("fonts/arial.ttf", 65);
    settingTexture = loadImage(renderer, "img/setting.png");
    moveSound = Mix_LoadWAV("sound/move.wav");
    kickSound = Mix_LoadWAV("sound/kick.wav");
    startSound = Mix_LoadWAV("sound/start.wav");
    winSound = Mix_LoadWAV("sound/win.wav");
    menuSound = Mix_LoadWAV("sound/menu.wav");
    for (int i = 0; i < GRID_SIZE * GRID_SIZE - 1; ++i)
        arr[i] = i + 1;
    arr[GRID_SIZE * GRID_SIZE - 1] = 0;
    for (int i = 1; i <= GRID_SIZE * GRID_SIZE - 1; ++i)
    {
        std::string ss = "img/" + std::to_string(i) + ".png";
        textures.push_back(loadImage(renderer, ss.c_str()));
    }

    Mix_PlayChannel(-1, menuSound, 0);
}

void Game::run()
{
    SDL_Event e;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            handleEvent(e);
        }
        update();
        render();
        SDL_Delay(16); // ~60fps
    }
}
void Game::handleEvent(SDL_Event& e)
{
    if (e.type == SDL_QUIT) quit = true;
    if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        switch (state)
        {
        case STATE_START:
            handleStartClick(x,y);
            break;
        case STATE_PLAYING:
            handlePlayingClick(x,y);
            break;
        case STATE_SETTINGS:
            handleSettingsClick(x,y);
            break;
        case STATE_OVER:
            handleGameOverClick(x,y);
            break;
        }
    }
}
void Game::handleStartClick(int x,int y)
{
    if (x >= 300 && x <= 500 && y >= 200 && y <= 250)
    {
        if (startSound) Mix_PlayChannel(-1, startSound, 0);
        state = STATE_PLAYING;
        now=0;
        shuffleGrid(arr);
        while (!isSolvable(arr))
        {
            shuffleGrid(arr);
        }
        startTime = SDL_GetTicks();
        moves = 0;
    }
    else if (x >= 300 && x <= 570 && y >= 200 && y <= 320)
    {
        state = STATE_SETTINGS;
        now=1;
    }
    else if (x >= 300 && x <= 500 && y >= 200 && y <= 390)
    {
        quit=true;
    }

}
void Game::handlePlayingClick(int x,int y)
{
    int boardWidth = GRID_SIZE * TILE_SIZE;
    int boardHeight = GRID_SIZE * TILE_SIZE;
    int startX = (SCREEN_WIDTH - boardWidth) / 2;
    int startY = (SCREEN_HEIGHT - boardHeight) / 2;
    if (x >= SCREEN_WIDTH - 40 && x <= SCREEN_WIDTH - 10 && y >= 10 && y <= 40)
    {
        if (kickSound) Mix_PlayChannel(-1, kickSound, 0);
        state = STATE_SETTINGS;
    }
    else if (x >= startX && x <= startX + boardWidth && y >= startY && y <= startY + boardHeight)
    {
        int tile_y = (x - startX) / TILE_SIZE;
        int tile_x = (y - startY) / TILE_SIZE;
        int check=moves;
        moveTile(arr, tile_x, tile_y, moves);
        if (check<moves)
            Mix_PlayChannel(-1, moveSound, 0);
    }
    if(isGameOver(arr))
    {
        if (winSound) Mix_PlayChannel(-1, winSound, 0);
        state=STATE_OVER;
    }
}

void Game::handleSettingsClick(int x,int y)
{
    int start_y = (SCREEN_HEIGHT - 170) / 2;
    if (x >= (SCREEN_WIDTH - 120) / 2 && x <= (SCREEN_WIDTH + 120) / 2 && y >= start_y && y <= start_y + 50)
    {
        if (kickSound) Mix_PlayChannel(-1, kickSound, 0);
        if(now==1)  state=STATE_START;
        else state=STATE_PLAYING;
    }
    else if (x >= (SCREEN_WIDTH - 120) / 2 && x <= (SCREEN_WIDTH + 120) / 2 && y >= start_y + 60 && y <= start_y + 190)
    {
        if (kickSound) Mix_PlayChannel(-1, kickSound, 0);
        quit = true;
    }
}
void Game::handleGameOverClick(int x,int y)
{
    int playAgainButtonWidth = 120;
    int playAgainButtonHeight = 50;
    int playAgainButtonX = (SCREEN_WIDTH - playAgainButtonWidth) / 2;
    int playAgainButtonY = 300;
    if (x >= playAgainButtonX && x <= playAgainButtonX + playAgainButtonWidth && y >= playAgainButtonY && y <= playAgainButtonY + playAgainButtonHeight)
    {
        if (startSound) Mix_PlayChannel(-1, startSound, 0);
        state = STATE_PLAYING;
        shuffleGrid(arr);
        while (!isSolvable(arr))
        {
            shuffleGrid(arr);
        }
        startTime = SDL_GetTicks();
        moves = 0;
    }
}
void Game::update()
{
    if (state == STATE_PLAYING && isGameOver(arr))
    {
        if (winSound) Mix_PlayChannel(-1, winSound, 0);
        state = STATE_OVER;
    }
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);
    if (state == STATE_START)
    {
        TTF_Font* bigfont = TTF_OpenFont("fonts/arial.ttf", 65);
        int textW, textH;
        TTF_SizeText(bigfont, "15 Puzzle", &textW, &textH);
        drawText(renderer, bigfont,"15 Puzzle", (SCREEN_WIDTH-textW)/2, 80, {255, 255, 255});
        drawButton(renderer, font, 300, 200, 200, 50, "Start");
        drawButton(renderer, font, 300, 270, 200, 50, "Settings");
        drawButton(renderer, font, 300, 340, 200, 50, "Exit");
        TTF_CloseFont(bigfont);

    }
    else if (state == STATE_PLAYING)
    {
        drawGrid(renderer, textures, arr);
        drawSetting(renderer, settingTexture, SCREEN_WIDTH - 40, 10, 30, 30);
        string s = "Time: " + to_string((SDL_GetTicks() - startTime) / 1000) + "s";
        drawText(renderer, font, s.c_str(), 10, 10, {255, 255, 255});
        s = "Moves: " + to_string(moves);
        drawText(renderer, font, s.c_str(), 10, 40, {255, 255, 255});
        if (isGameOver(arr))
            state=STATE_OVER;
    }
    else if(state==STATE_OVER)
    {
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
    else if (state == STATE_SETTINGS)
    {
        int start_y = (SCREEN_HEIGHT - 170) / 2;
        drawButton(renderer, font, (SCREEN_WIDTH - 120) / 2, start_y, 120, 50, "Back");
        drawButton(renderer, font, (SCREEN_WIDTH - 120) / 2, start_y + 60, 120, 50, "Exit");
    }
    SDL_RenderPresent(renderer);
}
void Game::cleanUp()
{
    for (auto tex : textures)
        SDL_DestroyTexture(tex);
    textures.clear();
    if (settingTexture)
    {
        SDL_DestroyTexture(settingTexture);
        settingTexture = nullptr;
    }
    if (font)
    {
        TTF_CloseFont(font);
        font = nullptr;
    }
    if (bigfont)
    {
        TTF_CloseFont(bigfont);
        bigfont = nullptr;
    }
    Mix_FreeChunk(moveSound);
    Mix_FreeChunk(kickSound);
    Mix_FreeChunk(startSound);
    Mix_FreeChunk(winSound);
    Mix_CloseAudio();
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

}
