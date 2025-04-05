#include "graphics.h"
#include "board.h"
#include <SDL.h>
#include <iostream>

extern Board* board;

int main(int argc, char* argv[]) {
    bool running = true;
    SDL_Event event;
    int gameState = 0;

    init();

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (gameState == 1 && board != nullptr) {
                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    board->handleClick(event.motion.x, event.motion.y);
                }
            }
        }

        if (gameState == 0) {
            renderMenu();
            if (board != nullptr) {
                gameState = 1;
                startTime = SDL_GetTicks();
                stepCount = 0;
            }
        } else if (gameState == 1) {
            renderGame();
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    close();
    return 0;
}
