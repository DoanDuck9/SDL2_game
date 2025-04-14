#include "defs.h"
#include "board.h"
#include <random>
#include <algorithm>
#include<ctime>
using namespace std;

void shuffleGrid(vector<int> &arr)
{
    random_device rd;
    mt19937 gen(rd());
    if (rd.entropy() == 0.0) {
        gen.seed(time(0));
    }
    shuffle(arr.begin(), arr.end(), gen);
}
bool isGameOver(vector <int> arr,int GRID_SIZE)
{
    int count = 1;
    for (int i = 0; i < GRID_SIZE * GRID_SIZE - 1; ++i)
    {
        if (arr[i] != count)
        {
            return false;
        }
        count++;
    }
    return true;
}

void moveTile(vector<int> &arr, int tile_x, int tile_y, int& moves,int GRID_SIZE)
{
    int index = tile_x * GRID_SIZE + tile_y;
    int index0 = -1;
    for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++)
    {
        if (arr[i] == 0)
        {
            index0 = i;
            break;
        }
    }

    int empty_x = index0 / GRID_SIZE;
    int empty_y = index0 % GRID_SIZE;

    if (tile_x == empty_x)
    {
        if (tile_y < empty_y)
        {
            for (int col = empty_y; col > tile_y; col--)
            {
                int i = tile_x * GRID_SIZE + col;
                swap(arr[i], arr[i-1]);
            }
            moves++;
        }
        else if (tile_y > empty_y)
        {
            for (int col = empty_y; col < tile_y; col++)
            {
                int i = tile_x * GRID_SIZE + col;
                swap(arr[i], arr[i+1]);
            }
            moves++;
        }
    }
    else if (tile_y == empty_y)
    {
        if (tile_x < empty_x)
        {
            for (int row = empty_x; row > tile_x; row--)
            {
                int i = row * GRID_SIZE + tile_y;
                swap(arr[i], arr[i-GRID_SIZE]);
            }
            moves++;
        }
        else if (tile_x > empty_x)
        {
            for (int row = empty_x; row < tile_x; row++)
            {
                int i = row * GRID_SIZE + tile_y;
                swap(arr[i], arr[i+GRID_SIZE]);
            }
            moves++;
        }
    }
}

bool isSolvable(vector<int> arr, int GRID_SIZE)
{
    int inv_count = 0;
    for (int i = 0; i < GRID_SIZE * GRID_SIZE - 1; ++i)
    {
        for (int j = i + 1; j < GRID_SIZE * GRID_SIZE; ++j)
        {
            if (arr[j] && arr[i] && arr[i] > arr[j])
            {
                inv_count++;
            }
        }
    }
    if (GRID_SIZE % 2 == 1)
        return (inv_count % 2 == 0);
    else
    {
        int zero_row;
        for (int i = 0; i < GRID_SIZE * GRID_SIZE; ++i)
        {
            if (arr[i] == 0)
            {
                zero_row = i / GRID_SIZE;
                break;
            }
        }
        return ((inv_count + zero_row) % 2 == 1);
    }
}
