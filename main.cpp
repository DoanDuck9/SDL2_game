#include "graphics.h"
#include <cstdlib>
using namespace std;
int main(int argc, char* argv[])
{
    int n;
    nhap(n);
    int tileSize =round(400 / n);
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    if (!init(window, renderer))
        return 1;
    string path="board/board_"+to_string(n)+".png";
    SDL_Texture* bgTexture = loadTexture(path.c_str(), renderer);
    path="image/pic"+to_string(n)+".png";
    SDL_Texture* origil=loadTexture(path.c_str(),renderer);
    if (!bgTexture||!origil)
    {
        cout << "Loi tai anh nen!" << endl;
        return 1;
    }
    vector<int> board(n * n);
    iota(board.begin(), board.end(), 0);
    srand(static_cast<unsigned>(time(0)));
    while (!inven(board) || checkWin(board, n))
        random_shuffle(board.begin(), board.end());
    vector<SDL_Texture*> numTex;
    upimg(numTex,renderer, n);
    bool quit = false;
    SDL_Event e;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                quit = true;
            else if (e.type == SDL_KEYDOWN)
            {
                int blankIndex = find(board.begin(), board.end(), 0) - board.begin();
                int row = blankIndex / n;
                int col = blankIndex % n;
                switch (e.key.keysym.sym)
                {
                case SDLK_UP:
                    if(row < n - 1)
                        swap(board[blankIndex], board[blankIndex + n]);
                    break;
                case SDLK_DOWN:
                    if(row>0)
                        swap(board[blankIndex], board[blankIndex - n]);
                    break;
                case SDLK_LEFT:
                    if(col<n-1)
                        swap(board[blankIndex], board[blankIndex + 1]);
                    break;
                case SDLK_RIGHT:
                    if(col>0)
                        swap(board[blankIndex], board[blankIndex - 1]);
                    break;
                case SDLK_RETURN:
                {
                    quit=true;
                    break;
                }
                }
                if (checkWin(board, n))
                {
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Chúc mừng!", "Bạn thắng game!", window);
                    quit = true;
                }
            }
        }
        renderGame(renderer, board, numTex, n, tileSize, bgTexture,origil);
        SDL_Delay(16);
    }
    close(window, renderer, numTex);
    return 0;
}
