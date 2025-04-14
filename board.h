#ifndef BOARD_H
#define BOARD_H
#include<vector>
using namespace std;
bool isGameOver(vector <int> arr,int GRID_SIZE);
void shuffleGrid(vector<int> &arr);
void moveTile(vector<int> &arr, int tile_x, int tile_y,int &moves,int GRID_SIZE);
bool isSolvable(vector<int> arr,int GRID_SIZE);

#endif
