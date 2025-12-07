#ifndef TETRIS_MINO_HPP
#define TETRIS_MINO_HPP

#include <pos.hpp>
#include <color.hpp>
#include <SDL2/SDL.h>

class Mino
{
private:
    Color m_color;
    char m_value;

    // Animation
    float animationProgress = 0.0;
    
public:
    Mino();
    Mino(char val);
    Color &color();
    const Color &color() const;
    char &value();
    const char &value() const;
    void draw(SDL_Renderer *renderer, int x, int y);
    void incrementAnimationProgress();
    bool animationEnded();
};

#endif