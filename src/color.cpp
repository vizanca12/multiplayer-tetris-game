#include <color.hpp>

Color::Color(uint8_t red, uint8_t blue, uint8_t green, uint8_t alpha)
{
    m_red = red;
    m_blue = blue;
    m_green = green;
    m_alpha = alpha;
}

Color::Color(uint32_t hex)
{
    m_red = (uint8_t)((hex & 0xFF000000) >> 24);
    m_green = (uint8_t)((hex & 0x00FF0000) >> 16);
    m_blue = (uint8_t)((hex & 0x0000FF00) >> 8);
    m_alpha = (uint8_t)(hex & 0x000000FF);
}

Color::Color()
{
    m_red = 0;
    m_green = 0;
    m_blue = 0;
    m_alpha = 0;
}

uint8_t Color::getRed()
{
    return m_red;
}

uint8_t Color::getBlue()
{
    return m_blue;
}

uint8_t Color::getGreen()
{
    return m_green;
}

uint8_t Color::getAlpha()
{
    return m_alpha;
}

void Color::set(uint8_t red, uint8_t blue, uint8_t green, uint8_t alpha)
{
    m_red = red;
    m_blue = blue;
    m_green = green;
    m_alpha = alpha;
}

void Color::setAlpha(uint8_t alpha)
{
    m_alpha = alpha;
}

Color Color::fromTetrimino(char c)
{
    switch (c)
    {
    case 'O':
        return YELLOW;
        break;
    case 'I':
        return CYAN;
        break;
    case 'T':
        return PURPLE;
        break;
    case 'L':
        return ORANGE;
        break;
    case 'J':
        return BLUE;
        break;
    case 'S':
        return GREEN;
        break;
    case 'Z':
        return RED;
        break;
    case 'G':
        return WHITE_TRANSPARENT;
        break;
    case 'W':
        return GREY;
        break;
    default:
        return BLACK;
        break;
    }

    return BLACK;
}