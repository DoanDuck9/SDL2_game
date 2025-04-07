#include "defs.h"
#include "board.h"
#include <random>
#include <algorithm>

using namespace std;

void shuffleGrid(int arr[]) {
    random_device rd;
    mt19937 g(rd());
    shuffle(arr, arr + GRID_SIZE * GRID_SIZE, g);
}

bool isGameOver(int arr[]) {
    int count = 1;
    for (int i = 0; i < GRID_SIZE * GRID_SIZE - 1; ++i) {
        if (arr[i] != count) {
            return false;
        }
        count++;
    }
    return true;
}

void moveTile(int arr[], int tile_x, int tile_y) {
    int index = tile_x * GRID_SIZE + tile_y;
    int emptyIndex = -1;

    if (tile_x > 0 && arr[(tile_x - 1) * GRID_SIZE + tile_y] == 0) {
        emptyIndex = (tile_x - 1) * GRID_SIZE + tile_y;
    } else if (tile_x < GRID_SIZE - 1 && arr[(tile_x + 1) * GRID_SIZE + tile_y] == 0) {
        emptyIndex = (tile_x + 1) * GRID_SIZE + tile_y;
    } else if (tile_y > 0 && arr[tile_x * GRID_SIZE + tile_y - 1] == 0) {
        emptyIndex = tile_x * GRID_SIZE + tile_y - 1;
    } else if (tile_y < GRID_SIZE - 1 && arr[tile_x * GRID_SIZE + tile_y + 1] == 0) {
        emptyIndex = tile_x * GRID_SIZE + tile_y + 1;
    }

    if (emptyIndex != -1) {
        swap(arr[index], arr[emptyIndex]);
    }
}

bool isSolvable(int arr[]) {
    int inv_count = 0;
    for (int i = 0; i < GRID_SIZE * GRID_SIZE - 1; ++i) {
        for (int j = i + 1; j < GRID_SIZE * GRID_SIZE; ++j) {
            if (arr[j] && arr[i] && arr[i] > arr[j]) {
                inv_count++;
            }
        }
    }
    if (GRID_SIZE % 2 == 1) {
        return (inv_count % 2 == 0);
    } else {
        int zero_row;
        for (int i = 0; i < GRID_SIZE * GRID_SIZE; ++i) {
            if (arr[i] == 0) {
                zero_row = i / GRID_SIZE;
                break;
            }
        }
        return ((inv_count + zero_row) % 2 == 1);
    }
}

bool checkWin(int arr[]) {
    int count = 1;
    for (int i = 0; i < GRID_SIZE * GRID_SIZE; ++i) {
        if (arr[i] != count) {
            if (i == GRID_SIZE * GRID_SIZE - 1 && arr[i] == 0) {
                return true;
            }
            return false;
        }
        count++;
    }
    return true;
}
