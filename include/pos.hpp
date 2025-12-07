#ifndef TETRIS_POS_HPP
#define TETRIS_POS_HPP

class Pos
{
private:
    int m_row;
    int m_col;

public:
    Pos();
    Pos(int row, int col);
    ~Pos();
    int &row();
    const int &row() const;
    int &col();
    const int &col() const;

    Pos &operator+(Pos &anotherPos);
    Pos &operator-(Pos &anotherPos);
    Pos &operator+=(Pos &anotherPos);
    Pos &operator-=(Pos &anotherPos);
};

#endif