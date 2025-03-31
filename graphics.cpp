#include "graphics.h"
void nhap(int &n)
{
    cout << "nhap so hang mong muon: ";
    cin >> n;
    while (n <= 2 || n > 10)
    {
        cout << "so hang nam trong khoang [3,10]: ";
        cin >> n;
    }
}
SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer)
{
    SDL_Surface* surface = IMG_Load(path);
    if (!surface)
    {
        cout << "Không load được ảnh (" << path << "): " << IMG_GetError() << endl;
        return nullptr;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return tex;
}
void close(SDL_Window* window, SDL_Renderer* renderer, vector<SDL_Texture*>& numTex)
{
    for (auto tex : numTex)
    {
        SDL_DestroyTexture(tex);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}
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
bool checkWin(const vector<int>& board, int n)
{
    for (int i = 0; i < n * n - 1; i++)
    {
        if (board[i] != i + 1)
            return false;
    }
    return true;
}
bool inven(const vector<int>& v)
{
    int count=0,dem=0;
    for(int i=0; i<v.size(); i++)
    {
        if(v[i]==0)
            dem=1;
        if(v[i]+dem==i+1)
            count++;
    }
    if(count==v.size())
        return false;
    count = 0;
    int n = sqrt(v.size());
    int k = 0;
    for (int i = 0; i < v.size(); i++)
    {
        if (v[i] == 0)
        {
            k = n - (i / n);
            continue;
        }
        for (int j = i + 1; j < v.size(); j++)
        {
            if (v[j] != 0 && v[i] > v[j])
                count++;
        }
    }
    if (n % 2 != 0)
        return (count % 2 == 0);
    else
        return ((k + count) % 2 != 0);
}
void renderGame(SDL_Renderer* renderer, const vector<int>& board, const vector<SDL_Texture*>& numTex, int n, int tileSize, SDL_Texture* bgTexture,SDL_Texture* origil)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    if( origil)
    {
        SDL_Rect bgRect= {600,Y,200,200};
        SDL_RenderCopy(renderer, origil, NULL, &bgRect);
    }
    if (bgTexture)
    {
        SDL_Rect bgRect = {X, Y, 400, 400};
        SDL_RenderCopy(renderer, bgTexture, NULL, &bgRect);
    }
    for (int r  = 0; r < n; r++)
    {
        for (int c = 0; c < n; c++)
        {
            int num = board[r * n + c];
            if (num != 0)
            {
                SDL_Texture* tex = numTex[num - 1];
                if (tex)
                {
                    SDL_Rect dstRect = {  c * tileSize+X, r * tileSize+Y, tileSize, tileSize };
                    SDL_RenderCopy(renderer, tex, NULL, &dstRect);
                }
            }
        }
    }
    SDL_RenderPresent(renderer);
}
void upimg(vector<SDL_Texture*> &numTex,SDL_Renderer *renderer,int n)
{
    for (int i = 1; i <= n * n; i++)
    {
        string path = "image/"+to_string(n)+"/img-" + to_string(i) + ".png";
        SDL_Texture* tex = loadTexture(path.c_str(), renderer);
        numTex.push_back(tex);
    }
}
