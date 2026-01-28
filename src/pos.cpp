#include <pos.hpp>
#include <iostream>

Pos::Pos(int row, int col)
{
    m_row = row;
    m_col = col;
}

Pos::Pos()
{
    m_row = 0;
    m_col = 0;
}

Pos::~Pos()
{
    // std::cout << "pos "
    //           << "(" << m_row << ", " << m_col << ") destroyed" << std::endl;
}

int &Pos::row()
{
    return m_row;
}

const int &Pos::row() const
{
    return m_row;
}

int &Pos::col()
{
    return m_col;
}

const int &Pos::col() const
{
    return m_col;
}

Pos &Pos::operator+(Pos &anotherPos)
{
    Pos *n_pos = new Pos(m_row, m_col);
    n_pos->m_row += anotherPos.m_row;
    n_pos->m_col += anotherPos.m_col;
    return *n_pos;
}

Pos &Pos::operator-(Pos &anotherPos)
{
    Pos *n_pos = new Pos(m_row, m_col);
    n_pos->m_row -= anotherPos.m_row;
    n_pos->m_col -= anotherPos.m_col;
    return *n_pos;
}

Pos &Pos::operator+=(Pos &anotherPos)
{
    m_row += anotherPos.m_row;
    m_col += anotherPos.m_col;
    return *this;
}

Pos &Pos::operator-=(Pos &anotherPos)
{
    m_row -= anotherPos.m_row;
    m_col -= anotherPos.m_col;
    return *this;
}
