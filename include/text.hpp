#ifndef TETRIS_TEXT_HPP
#define TETRIS_TEXT_HPP

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <string>
#include <chrono>

using namespace std;

class Text
{
public:
    Text(int fontSize);
    ~Text();
    void drawStart(int x, int y, string s, SDL_Renderer *renderer);
    void drawCenter(int startX, int startY, int endX, int endY, string s, SDL_Renderer *renderer);
    void drawEnd(int x, int y, string s, SDL_Renderer *renderer);

private:
    TTF_Font *font;
    int m_count = 0;
};

#endif
