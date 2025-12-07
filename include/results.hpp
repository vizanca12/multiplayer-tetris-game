#ifndef TETRIS_RESULTS_HPP
#define TETRIS_RESULTS_HPP

#include <SDL2/SDL.h>
#include <text.hpp>
#include <tetrimino.hpp>
#include <background.hpp>

using namespace std;

class Results
{
public:
    Results(int score, int linesCleared, bool win);
    void input();
    void render(int score, int linesCleared, bool win);
    void close();

private:
    SDL_Renderer *renderer;
    SDL_Window *window;
    Background *bg;
    Text *textSmall, *textSmaller, *textBig;

    bool menuShowing;
};

#endif