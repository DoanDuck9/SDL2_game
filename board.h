#ifndef BOARD_H
#define BOARD_H

#include "defs.h"

class Board {
public:
    Board(int size);
    void render(SDL_Renderer* renderer);
    void shuffle();
    SDL_Rect getTileRect(int row, int col);
    void handleClick(int x, int y);
    bool isWin();
    int getMoveCount();

private:
    int size;
    std::vector<int> tiles;
    int moveCount;
    void swapTile(int index1, int index2);
};

#endif
