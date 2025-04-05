#include "board.h"
#include <algorithm>
#include <random>
#include <sstream>

Board::Board(int s) {
    size = s;
    tiles.resize(size * size);
    for (int i = 0; i < size * size; ++i) {
        tiles[i] = i;
    }
    moveCount = 0;
}

SDL_Rect Board::getTileRect(int row, int col) {
    int boardSize = 400; // Kích thước bảng là 400x400
    int tileSize = boardSize / size;
    int boardX = (SCREEN_WIDTH - boardSize) / 2;
    int boardY = (SCREEN_HEIGHT - boardSize) / 2 + 50; // Điều chỉnh vị trí bảng

    SDL_Rect rect = {boardX + col * tileSize, boardY + row * tileSize, tileSize, tileSize};
    return rect;
}

void Board::render(SDL_Renderer* renderer) {
    for (int i = 0; i < size * size; ++i) {
        int row = i / size;
        int col = i % size;
        SDL_Rect rect = getTileRect(row, col);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &rect);

        if (tiles[i] != 0) {
            std::stringstream ss;
            ss << tiles[i];
            SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, ss.str().c_str(), {255, 255, 255, 255});
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_Rect textRect = {rect.x + rect.w / 2 - textSurface->w / 2, rect.y + rect.h / 2 - textSurface->h / 2, textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }
    }
}

void Board::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(tiles.begin(), tiles.end(), g);
}

void Board::handleClick(int x, int y) {
    int boardSize = 400;
    int tileSize = boardSize / size;
    int boardX = (SCREEN_WIDTH - boardSize) / 2;
    int boardY = (SCREEN_HEIGHT - boardSize) / 2 + 50;
    int col = (x - boardX) / tileSize;
    int row = (y - boardY) / tileSize;
    if (row < 0 || row >= size || col < 0 || col >= size) return;
    int idx = row * size + col;
    int blank = -1;
    for (std::vector<int>::size_type i = 0; i < tiles.size(); ++i) {
        if (tiles[i] == 0) {
            blank = static_cast<int>(i);
            break;
        }
    }
    int r2 = blank / size;
    int c2 = blank % size;
    if ((abs(r2 - row) == 1 && c2 == col) || (abs(c2 - col) == 1 && r2 == row)) {
        swapTile(idx, blank);
        moveCount++;
    }
}

void Board::swapTile(int i, int j) {
    std::swap(tiles[i], tiles[j]);
}

bool Board::isWin() {
    for (std::vector<int>::size_type i = 0; i < tiles.size() - 1; ++i) {
        if (tiles[i] != static_cast<int>(i) + 1) return false;
    }
    return tiles.back() == 0;
}

int Board::getMoveCount() {
    return moveCount;
}
