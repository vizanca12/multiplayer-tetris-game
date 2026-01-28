#ifndef TETRIS_BACKGROUND_HPP
#define TETRIS_BACKGROUND_HPP

#include <SDL2/SDL.h>
#include <text.hpp>
#include <vector>
#include <tetrimino.hpp>
#include <memory>

class Background
{
public:
    Background(SDL_Renderer *renderer);
    ~Background() = default; 

    void render();
    void tick();
    void generateNewBgTetrimino();
    void fallTetriminos();

private:
    std::vector<char> virtualBag = {'I', 'O', 'T', 'L', 'J', 'Z', 'S'};
    // Vector de ponteiros inteligentes (Sessão 4 + Sessão 11)
    std::vector<std::unique_ptr<Tetrimino>> tetriminosBg;
    
    time_t start_fall;
    time_t start_new;
    Uint32 startTime;
    SDL_Renderer *renderer;
};

#endif