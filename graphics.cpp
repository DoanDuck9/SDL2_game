#include "graphics.h"
#include "board.h"
#include <iostream>
#include <sstream>
#include <SDL_image.h>

using namespace std;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
TTF_Font* font = nullptr;

Board* board = nullptr;

Button* startBtn = nullptr;
Button* settingBtn = nullptr;
SDL_Color white = {255, 255, 255, 255};

Uint32 startTime = 0;
int stepCount = 0;

SDL_Texture* restartTexture = nullptr;
SDL_Texture* settingsTexture = nullptr;

void renderText(const std::string& text, int x, int y) {
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, text.c_str(), white);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect dstRect = {x, y, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, nullptr, &dstRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

Button::Button(const std::string& txt, int x, int y, TTF_Font* f, SDL_Color c, std::function<void()> click) {
    text = txt;
    font = f;
    color = c;
    onClick = click;
    SDL_Surface* surf = TTF_RenderText_Solid(font, text.c_str(), color);
    texture = SDL_CreateTextureFromSurface(renderer, surf);
    rect = {x, y, surf->w + 20, surf->h + 10};
    SDL_FreeSurface(surf);
}

void Button::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
    renderText(text, rect.x + 10, rect.y + 5);
}

void Button::handleEvent(SDL_Event* e) {
    if (e->type == SDL_MOUSEBUTTONDOWN) {
        int x = e->button.x;
        int y = e->button.y;
        if (x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h) {
            onClick();
        }
    }
}

void Button::destroy() {
    SDL_DestroyTexture(texture);
}

void renderMenu() {
    startBtn = new Button("Start", 250, 300, font, white, []() {
        board = new Board(4);
        board->shuffle();
    });

    bool quit = false;
    SDL_Event e;
    while (!quit && board == nullptr) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
            startBtn->handleEvent(&e);
        }
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderClear(renderer);
        startBtn->render(renderer);
        SDL_RenderPresent(renderer);
    }
}

void renderGame() {
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);

    renderText("Trò chơi xếp số", SCREEN_WIDTH / 2 - 80, 20);

    // Vị trí mới cho nút Restart (trên bảng)
    SDL_Rect restartRect = {SCREEN_WIDTH / 2 - 120, 100, 32, 32};
    SDL_RenderCopy(renderer, restartTexture, nullptr, &restartRect);

    // Vị trí mới cho nút Settings (góc trên bên phải)
    SDL_Rect settingsRect = {SCREEN_WIDTH - 50, 20, 32, 32};
    SDL_RenderCopy(renderer, settingsTexture, nullptr, &settingsRect);

    // Vị trí mới cho nút Luật chơi (góc trái giữa)
    Button lawButton("Luật chơi", 20, SCREEN_HEIGHT / 2 - 20, font, white, []() {
        // Xử lý sự kiện khi click vào nút Luật chơi
    });
    lawButton.render(renderer);

    if (board != nullptr) {
        board->render(renderer);

        Uint32 currentTime = SDL_GetTicks();
        float seconds = (currentTime - startTime) / 1000.0f;
        int minutes = (int)seconds / 60;
        int remainingSeconds = (int)seconds % 60;

        stringstream timeStream;
        timeStream << (minutes < 10 ? "0" : "") << minutes << ":" << (remainingSeconds < 10 ? "0" : "") << remainingSeconds;

        // Vị trí mới cho thông tin Thời gian (góc trái giữa)
        renderText("Thời gian", 20, SCREEN_HEIGHT / 2 + 20);
        renderText(timeStream.str(), 20, SCREEN_HEIGHT / 2 + 50);

        // Vị trí mới cho thông tin Số lượt (góc trái giữa)
        renderText("Số lượt", 20, SCREEN_HEIGHT / 2 + 90);
        renderText(to_string(stepCount), 20, SCREEN_HEIGHT / 2 + 120);

        if (board->isWin()) {
            renderText("Bạn đã thắng!", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2);
        }
    }

    SDL_RenderPresent(renderer);
}

void init() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);
    window = SDL_CreateWindow("Number Puzzle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    font = TTF_OpenFont("fonts/FreeSans.ttf", 24);

    restartTexture = IMG_LoadTexture(renderer, "img/random.png");
    settingsTexture = IMG_LoadTexture(renderer, "img/setting.png");
}

void close() {
    if (board) delete board;
    if (startBtn) startBtn->destroy();
    if (settingBtn) settingBtn->destroy();
    if (restartTexture) SDL_DestroyTexture(restartTexture);
    if (settingsTexture) SDL_DestroyTexture(settingsTexture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}
