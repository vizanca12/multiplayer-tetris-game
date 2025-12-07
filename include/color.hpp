#ifndef TETRIS_COLOR_HPP
#define TETRIS_COLOR_HPP

#define RED Color(0xFF0000FF)
#define YELLOW Color(0xFFFF00FF)
#define GREEN Color(0x00FF00FF)
#define BLUE Color(0X0000FFFF)
#define PURPLE Color(0x800080FF)
#define CYAN Color(0x00FFFFFF)
#define ORANGE Color(0xFF7F00FF)
#define GREY Color(0x808080FF);
#define BLACK Color(0x000000FF)
#define WHITE_TRANSPARENT Color(0xFFFFFF30);

#include <stdint.h>

class Color
{
public:
    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
    Color(uint32_t hex);
    Color();
    uint8_t getRed();
    uint8_t getGreen();
    uint8_t getBlue();
    uint8_t getAlpha();
    void setAlpha(uint8_t alpha);
    void set(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

    static Color fromTetrimino(char c);

private:
    uint8_t m_alpha;
    uint8_t m_red;
    uint8_t m_green;
    uint8_t m_blue;
};

#endif