#ifndef TETRIS_MENU_HPP
#define TETRIS_MENU_HPP

#include <SDL2/SDL.h>
#include <text.hpp>
#include <tetrimino.hpp>
#include <background.hpp>

using namespace std;

class Menu
{
public:
    Menu();
    void input();
    void render();
    void close();
    bool hasChosen();
    int getChoice();

private:
    SDL_Renderer *renderer;
    SDL_Window *window;
    Background *bg;
    Text *textSmall, *textSmaller, *textBig;

    bool hasMadeChoice;
    bool menuShowing;
    int choice;
};

#endif