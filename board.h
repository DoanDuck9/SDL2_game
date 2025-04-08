#ifndef BOARD_H
#define BOARD_H

bool isGameOver(int arr[]);
void shuffleGrid(int arr[]);
void moveTile(int arr[], int tile_x, int tile_y,int &moves);
bool isSolvable(int arr[]);
bool checkWin(int arr[]);

#endif
