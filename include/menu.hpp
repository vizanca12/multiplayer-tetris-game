#ifndef TETRIS_MENU_HPP
#define TETRIS_MENU_HPP

#include <SDL2/SDL.h>
#include <text.hpp>
#include <tetrimino.hpp>
#include <background.hpp>
#include <memory> // Necessário para unique_ptr

class Menu
{
public:
    Menu();
    ~Menu() = default; // Destrutor pertinente (Exigência do Barramento)

    void input();
    void render();
    void close();
    bool hasChosen();
    int getChoice();

private:
    SDL_Renderer *renderer; // Mantemos cru pois o SDL gerencia o renderer
    SDL_Window *window;     // Mantemos cru pelo mesmo motivo
    
    // Ponteiros Inteligentes 
    std::unique_ptr<Background> bg;
    std::unique_ptr<Text> textSmall;
    std::unique_ptr<Text> textSmaller;
    std::unique_ptr<Text> textBig;

    bool hasMadeChoice;
    bool menuShowing;
    int choice;
};

#endif