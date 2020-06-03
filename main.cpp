#include <SDL.h>
#include <SDL_image.h>
#include <bits/stdc++.h>

#undef main

using namespace std;

bool playAgain = true;
const int window_cell = 188;
const string imageLink[] = {"ava.jpg", "fox.jpg"};
const string wintext = "win phrase.png";

struct Graphic
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Texture* win;
    vector<SDL_Rect> sprite;
};

struct Board
{
    int table[3][3];
};

/*------------------------------------------*/

bool KhoitaoGraphic(Graphic &g);

SDL_Texture* createTexture(SDL_Renderer *renderer, const string &path);

void KhoitaoSprite(Graphic &g);

void KhoitaoBoard(Board &B);

SDL_Rect getSprite(Graphic &g, Board &B, int x, int y);

void DisplayGame(Graphic &g, Board &B);

void xuliEvent(Board &B, SDL_Event e);

void findBlank(Board &B, int &x, int &y);

bool Win(Board &B);

void drawYouWin(Graphic &g);

bool check(Board &B);

void hack(Board &B);

void close(Graphic &g);

void err(const string &m);

/*-------------------------------------------*/

int main()
{
    srand(time(NULL));
    Graphic g;
    if (KhoitaoGraphic(g) == false)
    {
        close(g);
        return 1;
    }

    while (playAgain==true)
    {
        Board B;
        KhoitaoBoard(B);
        while (check(B)) KhoitaoBoard(B);

        bool playing = true;
        SDL_Event main_event;

        while (playing == true)
        {
            DisplayGame(g, B);


            while (SDL_PollEvent(&main_event))
            {
                switch (main_event.type)
                {
                    case SDL_QUIT:
                    {
                        playing = false;
                        playAgain = false;
                        break;
                    }
                    case SDL_KEYDOWN:
                    {
                        if (main_event.key.keysym.sym == SDLK_0)
                        {
                            hack(B);
                        }
                        else if (Win(B) and main_event.key.keysym.sym == SDLK_RETURN)
                        {
                            g.texture = createTexture(g.renderer, imageLink[rand() % 2].c_str());
                            if (g.texture == NULL)
                            {
                                string m = SDL_GetError();
                                err(m);
                                return false;
                            }

                            playing = false;
                            break;
                        }
                        else xuliEvent(B, main_event);
                        break;
                    }
                    default: break;
                }
            }
        }
    }

    close(g);
    return 0;
}

/*--------------------------------------------------*/

bool KhoitaoGraphic(Graphic &g)
{
    g.renderer = NULL;
    g.window = NULL;
    g.texture = NULL;
    g.win = NULL;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        string m = SDL_GetError();
        err(m);
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        string m = SDL_GetError();
        err(m);
        return false;
    }

    g.window = SDL_CreateWindow("Xep hinh",
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                 3 * window_cell,
                                 3 * window_cell,
                                 SDL_WINDOW_SHOWN);
    if (g.window == NULL)
    {
        string m = SDL_GetError();
        err(m);
        return false;
    }

    g.renderer = SDL_CreateRenderer(g.window, -1, SDL_RENDERER_ACCELERATED);
    if (g.renderer == NULL)
    {
        string m = SDL_GetError();
        err(m) ;
        return false;
    }

    g.texture = createTexture(g.renderer, imageLink[rand() % 2].c_str());
    if (g.texture == NULL)
    {
        string m = SDL_GetError();
        err(m);
        return false;
    }

    g.win = createTexture(g.renderer, wintext);
    if (g.win == NULL)
    {
        string m = SDL_GetError();
        err(m);
        return false;
    }


    KhoitaoSprite(g);
    return true;
}

void KhoitaoBoard(Board &B)
{
    bool check[10];
    for(int i = 0; i <= 9; i ++) check[i] = false;

    for(int i = 0; i < 3; i ++)
    {
        int j = 0;
        while(j < 3)
        {
            int num_rand = 1 + rand() % 9;
            if (check[num_rand] == false)
            {
                B.table[i][j] = num_rand;
                check[num_rand] = true;
                j ++;
            }
        }
    }
}

bool Win(Board &B)
{
    for (int i = 0; i < 3; i ++)
    {
        for (int j = 0; j < 3; j ++)
        {
             if (B.table[i][j] != i*3 + j +1) return false;
        }
    }
    return true;
}

bool check(Board &B)
{
    int sum = 0;

    for (int i = 0; i < 9; i ++)
    {
        int tmp = 0;
        {
            if (B.table[i/3][i%3] != 9)
            {
                for (int j = i + 1; j < 9; j ++)
                {
                    if (B.table[j / 3][j % 3] < B.table[i / 3][i % 3]
                    and B.table[j / 3][j % 3] != 9)
                    {
                        tmp ++;
                    }
                }
                sum+=tmp;
            }
        }
    }

    if (sum % 2 == 1) return true;
    return false;
}


void drawYouWin(Graphic &g)
{
    SDL_Rect src = {0,0, 626, 626};
    SDL_Rect des = {100, 100, 188 * 2, 188 * 2};
    SDL_RenderCopy(g.renderer ,g.win, &src, &des);
}

SDL_Texture* createTexture(SDL_Renderer *renderer, const string &path)
{
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface == NULL)
    {
        err("Load image");
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}

void KhoitaoSprite(Graphic &g)
{
    for (int i = 1; i <= 9; i ++)
    {
        SDL_Rect rect = {0, 0, 188, 188};
        switch (i)
        {
        case 1:
            {
                rect.x = 0;
                rect.y = 0;
                break;
            }
        case 2:
            {
                rect.x = 188;
                rect.y = 0;
                break;
            }
        case 3:
            {
                rect.x = 376;
                rect.y = 0;
                break;
            }
        case 4:
            {
                rect.x = 0;
                rect.y = 188;
                break;
            }
        case 5:
            {
                rect.x = 188;
                rect.y = 188;
                break;
            }
        case 6:
            {
                rect.x = 376;
                rect.y = 188;
                break;
            }
        case 7:
            {
                rect.x = 0;
                rect.y = 376;
                break;
            }
        case 8:
            {
                rect.x = 188;
                rect.y = 376;
                break;
            }
        case 9:
            {
                rect.x = 376;
                rect.y = 376;
                break;
            }
        }
        g.sprite.push_back(rect);
    }
}

SDL_Rect getSprite(Graphic &g, Board &B, int x, int y)
{
   int num = B.table[x][y];
   return g.sprite[num - 1];
}

void DisplayGame(Graphic &g, Board &B)
{
    SDL_RenderClear(g.renderer);
    for (int i = 0; i < 3; i ++)
    {
        for (int j = 0; j < 3; j ++)
        {
            SDL_Rect rect_src = getSprite(g, B, i, j);
            SDL_Rect rect_des = {j * window_cell,
                                 i * window_cell,
                                 window_cell,
                                 window_cell};
            SDL_RenderCopy(g.renderer, g.texture, &rect_src, &rect_des);
        }
    }

    if (Win(B)) drawYouWin(g);

    SDL_RenderPresent(g.renderer);
}

void xuliEvent(Board &B, SDL_Event e)
{
    int row, col;
    findBlank(B, row, col);

    switch (e.key.keysym.sym)
    {
    case SDLK_UP:
        {
            if (row != 2)
            {
                swap(B.table[row][col], B.table[row + 1][col]);
            }
            break;
        }
    case SDLK_DOWN:
        {
            if (row != 0)
            {
                swap(B.table[row][col], B.table[row - 1][col]);
            }
            break;
        }
    case SDLK_LEFT:
        {
            if (col != 2)
            {
                swap(B.table[row][col], B.table[row][col + 1]);
            }
            break;
        }
    case SDLK_RIGHT:
        {
            if (col != 0)
            {
                swap(B.table[row][col], B.table[row][col - 1]);
            }
            break;
        }
    default: break;
    }
}

void findBlank(Board &B, int &x, int &y)
{
    for(int i = 0; i < 3; i ++)
    {
        for (int j = 0; j < 3; j ++)
        {
            if (B.table[i][j] == 9)
            {
                x = i;
                y = j;
                return;
            }
        }
    }
}

void hack(Board &B)
{
    for (int i = 0; i < 3; i ++)
    {
        for (int j = 0; j< 3; j ++)
        {
            B.table[i][j] = i*3 + j + 1;
        }
    }
}
void close(Graphic &g)
{
    SDL_DestroyWindow(g.window);
    SDL_DestroyRenderer(g.renderer);
    SDL_DestroyTexture(g.texture);
    SDL_DestroyTexture(g.win);

    IMG_Quit();
    SDL_Quit();
}


void err(const string &m)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", m.c_str(), NULL);
}

