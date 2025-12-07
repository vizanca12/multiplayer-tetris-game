

#include <mino.hpp>
#include <globals.hpp>

Mino::Mino()
{
    m_value = 0;
}

Mino::Mino(char val)
{
    m_color = Color::fromTetrimino(val);
    m_value = val;
}

char &Mino::value()
{
    return m_value;
}

const char &Mino::value() const
{
    return m_value;
}

Color &Mino::color()
{
    return m_color;
}

const Color &Mino::color() const
{
    return m_color;
}

void Mino::draw(SDL_Renderer *renderer, int x, int y)
{
    if (m_value == 0)
        return;

    SDL_Rect rect;
    rect.x = x + 5;
    rect.y = y + 5;
    rect.w = TILE_SIZE - 5;
    rect.h = TILE_SIZE - 5;

    SDL_SetRenderDrawColor(renderer, m_color.getRed(), m_color.getGreen(), m_color.getBlue(), m_color.getAlpha() * (1 - animationProgress));
    SDL_RenderFillRect(renderer, &rect);

    rect.x = x;
    rect.y = y;
    rect.w = TILE_SIZE;
    rect.h = TILE_SIZE;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200 * (1 - animationProgress));
    SDL_RenderDrawRect(renderer, &rect);
}

void Mino::incrementAnimationProgress()
{
    animationProgress += ANIMATION_PROGRESS;
}

bool Mino::animationEnded()
{
    return animationProgress >= 1;
}
