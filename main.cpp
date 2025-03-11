#include <SDL.h>
#include <SDL_image.h>
#include <bits/stdc++.h>
using namespace std;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int BOARD_START_X = 50;
const int BOARD_START_Y = 50;
bool init(SDL_Window*& window, SDL_Renderer*& renderer)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "SDL init thất bại: " << SDL_GetError() << endl;
        return false;
    }
    window = SDL_CreateWindow("Game Xếp Hình",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);
    if (!window)
    {
        cout << "Tạo cửa sổ thất bại: " << SDL_GetError() << endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        cout << "Tạo renderer thất bại: " << SDL_GetError() << endl;
        return false;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        cout << "SDL_image lỗi: " << IMG_GetError() << endl;
        return false;
    }
    return true;
}

void close(SDL_Window* win, SDL_Renderer* ren, vector<SDL_Texture*>& numTex)
{
    for (auto tex : numTex)
    {
        SDL_DestroyTexture(tex);
    }
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
}
SDL_Texture* loadTexture(const char* path, SDL_Renderer* ren)
{
    SDL_Surface* surface = IMG_Load(path);
    if (!surface)
    {
        cout << "Không load được ảnh (" << path << "): " << IMG_GetError() << endl;
        return nullptr;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surface);
    SDL_FreeSurface(surface);
    return tex;
}

bool checkWin(const vector<int>& board,int n)
{
    for (int i = 0; i < n*n-1; i++)
    {
        if (board[i] != i + 1)
            return false;
    }
    return true;
}
bool inven(const vector<int> v)
{
    int dem=0;
    for(int i=0; i<v.size(); i++)
        if(v[i]==i+1)
            dem++;
    if(dem!=0)
        return false;
    int x=0,so0=0;
    for(int i=0; i<v.size(); i++)
    {
        if(v[i]==0)
        {
            so0=sqrt(v.size())-i-1;
            continue;
        }
        for(int j=i+1; j<v.size(); j++)
        {
            if(v[j]==0)
                continue;
            if(v[i]>v[j])
                x++;

        }
    }
    if(v.size()%2==0)
        x+=so0;
    return (x%2==0);
}
int main(int argc, char* argv[])
{
    int n;
    cout<<"nhap so hang ban mong muon: ";
    cin>>n;
    while(n<=2||n>10)
    {
        if(n>10)
            cout<<"Hay nhap so nho hon: ";
        else if(n<=2)
            cout<<" Hay nhap so lon hon: ";
        cin>>n;
    }
    int wid=(SCREEN_HEIGHT-200)/n;
    //int wid=65;
    SDL_Window* win = nullptr;
    SDL_Renderer* ren = nullptr;
    if (!init(win, ren))
        return 1;
    vector<int> board;
    for(int i=0; i<n*n; i++)
        board.push_back(i);
    //vector<int> board = {1,2,3,4,5,6,7,8,0};
    srand(static_cast<unsigned int>(time(nullptr)));
    while(!inven(board))
        random_shuffle(board.begin(), board.end());
    vector<SDL_Texture*> numTex;
    for (int i = 1; i <= n*n; i++)
    {
        string path = "image/num" + to_string(i) + ".png";
        SDL_Texture* tex = loadTexture(path.c_str(), ren);
        if (!tex)
        {
            cout << "Lỗi tải ảnh: " << path << endl;
            close(win, ren, numTex);
            return 1;
        }
        numTex.push_back(tex);
    }
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
                int anh0 = find(board.begin(), board.end(), 0) - board.begin();
                int row = anh0 / n;
                int col = anh0 % n;
                if (e.key.keysym.sym == SDLK_UP)
                {
                    if (row < n - 1)
                    {
                        swap(board[anh0], board[anh0 + n]);
                    }
                }
                else if (e.key.keysym.sym == SDLK_DOWN)
                {
                    if (row > 0)
                    {
                        swap(board[anh0], board[anh0 - n]);
                    }
                }
                else if (e.key.keysym.sym == SDLK_LEFT)
                {
                    if (col < n - 1)
                    {
                        swap(board[anh0], board[anh0+1]);
                    }
                }
                else if (e.key.keysym.sym == SDLK_RIGHT)
                {
                    if (col > 0)
                    {
                        swap(board[anh0], board[anh0-1]);
                    }
                }
                if (checkWin(board,n))
                {
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Chúc mừng!", "Bạn thắng game!", win);
                    quit = true;
                }
                else if(e.key.keysym.sym==SDLK_RETURN)
                    quit= true;
            }
        }
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        for (int r = 0; r < n; r++)
        {
            for (int c = 0; c < n; c++)
            {
                SDL_Rect cell = { BOARD_START_X + c * wid,BOARD_START_Y + r * wid,wid,wid};
                SDL_RenderDrawRect(ren, &cell);
            }
        }
        for (int r = 0; r < n; r++)
        {
            for (int c = 0; c < n; c++)
            {
                int num = board[r * n + c];
                if (num != 0)
                {
                    SDL_Texture* tex = numTex[num - 1];
                    if (tex)
                    {
                        int texW, texH;
                        SDL_QueryTexture(tex, NULL, NULL, &texW, &texH);
                        SDL_Rect rect;
                        rect.x = BOARD_START_X + c * wid + (wid - texW) / 2;
                        rect.y = BOARD_START_Y + r * wid + (wid - texH) / 2;
                        rect.w = texW;
                        rect.h = texH;
                        SDL_RenderCopy(ren, tex, NULL, &rect);
                    }
                }
            }
        }
        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }
    close(win, ren, numTex);
    return 0;
}
