#include <background.hpp>
#include <globals.hpp>
#include <ctime>

Background::Background(SDL_Renderer *renderer)
{
    this->renderer = renderer;
    for (int i = 0; i < 7; i++)
    {
        generateNewBgTetrimino();
        fallTetriminos();
        fallTetriminos();
        fallTetriminos();
    }

    start_fall = time(nullptr);
    start_new = time(nullptr);
}

void Background::tick()
{
    time_t now_s = time(nullptr);

    if (now_s - start_fall >= 1)
    {
        fallTetriminos();
        start_fall = now_s;
    }
    if (now_s - start_new >= 3)
    {
        generateNewBgTetrimino();
        start_new = now_s;
    }
}

void Background::generateNewBgTetrimino()
{
    int ti = rand() % virtualBag.size();
    char name = virtualBag.at(ti);
    virtualBag.erase(virtualBag.begin() + ti);

    if (virtualBag.empty())
    {
        virtualBag = VIRTUAL_BAG;
    }

    Tetrimino *t = Tetrimino::generateTetriminoFrom(name);
    int col = rand() % 10;
    Pos p(-2, col);
    t->pos() = p;
    tetriminosBg.push_back(t);
}

void Background::fallTetriminos()
{
    for (int i = 0; i < tetriminosBg.size();)
    {
        Tetrimino *t = tetriminosBg[i];
        t->moveAction(MOVE_DOWN);
        if (t->pos().row() >= 20)
            tetriminosBg.erase(tetriminosBg.begin() + 0);
        else
            i++;
    }
}

void Background::render()
{
    for (int i = 0; i < tetriminosBg.size(); i++)
    {
        Tetrimino *t = tetriminosBg[i];
        int x = t->pos().col() * TILE_SIZE;
        int y = t->pos().row() * TILE_SIZE;
        t->draw(renderer, x, y);
    }

    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = WINDOW_WIDTH_MENU;
    rect.h = WINDOW_HEIGHT_MENU;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
    SDL_RenderFillRect(renderer, &rect);
}