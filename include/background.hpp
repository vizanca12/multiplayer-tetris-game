#ifndef TETRIS_BACKGROUND_HPP
#define TETRIS_BACKGROUND_HPP

#include <SDL2/SDL.h>
#include <text.hpp>
#include <vector>
#include <tetrimino.hpp>

using namespace std;

class Background
{
public:
    Background(SDL_Renderer *renderer);
    void render();
    void tick();
    void generateNewBgTetrimino();
    void fallTetriminos();

private:
    vector<char> virtualBag = {'I', 'O', 'T', 'L', 'J', 'Z', 'S'};
    vector<Tetrimino *> tetriminosBg;
    time_t start_fall;
    time_t start_new;
    Uint32 startTime;
    SDL_Renderer *renderer;
};

#endif