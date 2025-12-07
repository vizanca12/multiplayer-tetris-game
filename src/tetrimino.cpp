#include <tetrimino.hpp>
#include <color.hpp>
#include <globals.hpp>
#include <mino.hpp>
#include <iostream>

Tetrimino::Tetrimino()
{
    reset();
}

void Tetrimino::reset()
{
    m_orientation = N;
    m_pos = Pos(-2, MATRIX_WIDTH / 2 - 1);
    dead = false;
}

TetriminoI::TetriminoI() : Tetrimino()
{
    m_color = CYAN;
    t_name = 'I';
};

TetriminoJ::TetriminoJ() : Tetrimino()
{
    m_color = BLUE;
    t_name = 'J';
};

TetriminoL::TetriminoL() : Tetrimino()
{
    m_color = ORANGE;
    t_name = 'L';
};

TetriminoO::TetriminoO() : Tetrimino()
{
    m_color = YELLOW;
    t_name = 'O';
};

TetriminoT::TetriminoT() : Tetrimino()
{
    m_color = PURPLE;
    t_name = 'T';
};

TetriminoS::TetriminoS() : Tetrimino()
{
    m_color = GREEN;
    t_name = 'S';
};

TetriminoZ::TetriminoZ() : Tetrimino()
{
    m_color = RED;
    t_name = 'Z';
};

Tetrimino *Tetrimino::generateTetriminoFrom(char c)
{
    switch (c)
    {
    case 'I':
        return new TetriminoI();
        break;
    case 'O':
        return new TetriminoO();
        break;
    case 'T':
        return new TetriminoT();
        break;
    case 'L':
        return new TetriminoL();
        break;
    case 'J':
        return new TetriminoJ();
        break;
    case 'Z':
        return new TetriminoZ();
        break;
    case 'S':
        return new TetriminoS();
        break;
    default:
        break;
    }

    return new TetriminoI();
}

void Tetrimino::rotateAction(TetriminoAction rot, int point)
{
    prev_pos = m_pos;
    prev_orientation = m_orientation;

    int dir = (rot == ROTATE_LEFT ? 1 : -1);

    m_orientation = Orientation((4 + m_orientation + dir) % 4);

    auto rotationMap = getRotationMap();

    Pos pos_initial = rotationMap[m_orientation][point];  // Pos(3,1)
    Pos pos_final = rotationMap[prev_orientation][point]; // Pos(1,0)
    Pos shift = pos_final - pos_initial;                  // Pos(-2, -1)

    m_pos += shift;
}

void Tetrimino::moveAction(TetriminoAction mov)
{
    prev_pos = m_pos;
    prev_orientation = m_orientation;

    switch (mov)
    {
    case MOVE_LEFT:
        m_pos.col() -= 1;
        break;
    case MOVE_RIGHT:
        m_pos.col() += 1;
        break;
    case MOVE_DOWN:
        m_pos.row() += 1;
        break;
    case MOVE_UP:
        m_pos.row() -= 1;
        break;
    default:
        break;
    }
}

void Tetrimino::undoPreviousAction()
{
    m_pos = prev_pos;
    m_orientation = prev_orientation;
}

Pos &Tetrimino::pos()
{
    return m_pos;
}

const Pos &Tetrimino::pos() const
{
    return m_pos;
}

char &Tetrimino::name()
{
    return t_name;
}

const char &Tetrimino::name() const
{
    return t_name;
}

void Tetrimino::draw(SDL_Renderer *renderer)
{
    if (dead)
        return;

    array<Pos, 4> posMinos = getMinos();

    for (int i = 0; i < 4; i++)
    {
        Pos p = m_pos + posMinos[i];
        Mino mino(t_name);
        int x = p.col() * TILE_SIZE + TETRIS_MAP_INIT_X;
        int y = p.row() * TILE_SIZE + TETRIS_MAP_INIT_Y;
        mino.draw(renderer, x, y);
    }
}

void Tetrimino::draw(SDL_Renderer *renderer, int x, int y)
{
    if (dead)
        return;

    array<Pos, 4> posMinos = getMinos();

    for (int i = 0; i < 4; i++)
    {
        Pos p = posMinos[i];
        Mino mino(t_name);
        int mx = p.col() * TILE_SIZE + x;
        int my = p.row() * TILE_SIZE + y;
        mino.draw(renderer, mx, my);
    }
}

void Tetrimino::setDead()
{
    dead = true;
}

array<Pos, 4> TetriminoJ::getMinos()
{
    array<Pos, 4> minos;

    switch (m_orientation)
    {
    case N:
    {
        minos[0] = Pos(0, 0);
        minos[1] = Pos(1, 0);
        minos[2] = Pos(1, 1);
        minos[3] = Pos(1, 2);
    }
    break;
    case E:
    {
        minos[0] = Pos(0, 1);
        minos[1] = Pos(0, 2);
        minos[2] = Pos(1, 1);
        minos[3] = Pos(2, 1);
    }
    break;
    case S:
    {
        minos[0] = Pos(1, 0);
        minos[1] = Pos(1, 1);
        minos[2] = Pos(1, 2);
        minos[3] = Pos(2, 2);
    }
    break;
    case W:
    {
        minos[0] = Pos(0, 1);
        minos[1] = Pos(1, 1);
        minos[2] = Pos(2, 0);
        minos[3] = Pos(2, 1);
    }
    break;
    default:
        break;
    }

    return minos;
}

array<Pos, 4> TetriminoI::getMinos()
{
    array<Pos, 4> minos;

    switch (m_orientation)
    {
    case N:
    {
        minos[0] = Pos(1, 0);
        minos[1] = Pos(1, 1);
        minos[2] = Pos(1, 2);
        minos[3] = Pos(1, 3);
    }
    break;
    case E:
    {
        minos[0] = Pos(0, 2);
        minos[1] = Pos(1, 2);
        minos[2] = Pos(2, 2);
        minos[3] = Pos(3, 2);
    }
    break;
    case S:
    {
        minos[0] = Pos(2, 0);
        minos[1] = Pos(2, 1);
        minos[2] = Pos(2, 2);
        minos[3] = Pos(2, 3);
    }
    break;
    case W:
    {
        minos[0] = Pos(0, 1);
        minos[1] = Pos(1, 1);
        minos[2] = Pos(2, 1);
        minos[3] = Pos(3, 1);
    }
    break;
    default:
        break;
    }

    return minos;
}

array<Pos, 4> TetriminoL::getMinos()
{
    array<Pos, 4> minos;
    switch (m_orientation)
    {
    case N:
    {
        minos[0] = Pos(0, 2);
        minos[1] = Pos(1, 0);
        minos[2] = Pos(1, 1);
        minos[3] = Pos(1, 2);
    }
    break;
    case E:
    {
        minos[0] = Pos(0, 1);
        minos[1] = Pos(1, 1);
        minos[2] = Pos(2, 1);
        minos[3] = Pos(2, 2);
    }
    break;
    case S:
    {
        minos[0] = Pos(1, 0);
        minos[1] = Pos(1, 1);
        minos[2] = Pos(1, 2);
        minos[3] = Pos(2, 0);
    }
    break;
    case W:
    {
        minos[0] = Pos(0, 0);
        minos[1] = Pos(0, 1);
        minos[2] = Pos(1, 1);
        minos[3] = Pos(2, 1);
    }
    break;
    default:
        break;
    }

    return minos;
}

array<Pos, 4> TetriminoO::getMinos()
{
    array<Pos, 4> minos;
    switch (m_orientation)
    {
    case N:
    {
        minos[0] = Pos(0, 0);
        minos[1] = Pos(0, 1);
        minos[2] = Pos(1, 0);
        minos[3] = Pos(1, 1);
    }
    break;
    case E:
    {
        minos[0] = Pos(0, 0);
        minos[1] = Pos(0, 1);
        minos[2] = Pos(1, 0);
        minos[3] = Pos(1, 1);
    }
    break;
    case S:
    {
        minos[0] = Pos(0, 0);
        minos[1] = Pos(0, 1);
        minos[2] = Pos(1, 0);
        minos[3] = Pos(1, 1);
    }
    break;
    case W:
    {
        minos[0] = Pos(0, 0);
        minos[1] = Pos(0, 1);
        minos[2] = Pos(1, 0);
        minos[3] = Pos(1, 1);
    }
    break;
    default:
        break;
    }

    return minos;
}

array<Pos, 4> TetriminoS::getMinos()
{
    array<Pos, 4> minos;
    switch (m_orientation)
    {
    case N:
    {
        minos[0] = Pos(0, 1);
        minos[1] = Pos(0, 2);
        minos[2] = Pos(1, 0);
        minos[3] = Pos(1, 1);
    }
    break;
    case E:
    {
        minos[0] = Pos(0, 1);
        minos[1] = Pos(1, 1);
        minos[2] = Pos(1, 2);
        minos[3] = Pos(2, 2);
    }
    break;
    case S:
    {
        minos[0] = Pos(1, 1);
        minos[1] = Pos(1, 2);
        minos[2] = Pos(2, 0);
        minos[3] = Pos(2, 1);
    }
    break;
    case W:
    {
        minos[0] = Pos(0, 0);
        minos[1] = Pos(1, 0);
        minos[2] = Pos(1, 1);
        minos[3] = Pos(2, 1);
    }
    break;
    default:
        break;
    }

    return minos;
}

array<Pos, 4> TetriminoT::getMinos()
{
    array<Pos, 4> minos;
    switch (m_orientation)
    {
    case N:
    {
        minos[0] = Pos(0, 1);
        minos[1] = Pos(1, 0);
        minos[2] = Pos(1, 1);
        minos[3] = Pos(1, 2);
    }
    break;
    case E:
    {
        minos[0] = Pos(0, 1);
        minos[1] = Pos(1, 1);
        minos[2] = Pos(1, 2);
        minos[3] = Pos(2, 1);
    }
    break;
    case S:
    {
        minos[0] = Pos(1, 0);
        minos[1] = Pos(1, 1);
        minos[2] = Pos(1, 2);
        minos[3] = Pos(2, 1);
    }
    break;
    case W:
    {
        minos[0] = Pos(0, 1);
        minos[1] = Pos(1, 0);
        minos[2] = Pos(1, 1);
        minos[3] = Pos(2, 1);
    }
    break;
    default:
        break;
    }

    return minos;
}

array<Pos, 4> TetriminoZ::getMinos()
{
    array<Pos, 4> minos;
    switch (m_orientation)
    {
    case N:
    {
        minos[0] = Pos(0, 0);
        minos[1] = Pos(0, 1);
        minos[2] = Pos(1, 1);
        minos[3] = Pos(1, 2);
    }
    break;
    case E:
    {
        minos[0] = Pos(0, 2);
        minos[1] = Pos(1, 1);
        minos[2] = Pos(1, 2);
        minos[3] = Pos(2, 1);
    }
    break;
    case S:
    {
        minos[0] = Pos(1, 0);
        minos[1] = Pos(1, 1);
        minos[2] = Pos(2, 1);
        minos[3] = Pos(2, 2);
    }
    break;
    case W:
    {
        minos[0] = Pos(0, 1);
        minos[1] = Pos(1, 0);
        minos[2] = Pos(1, 1);
        minos[3] = Pos(2, 0);
    }
    break;
    default:
        break;
    }

    return minos;
}

int TetriminoI::getSize()
{
    return 4;
}

int TetriminoJ::getSize()
{
    return 3;
}

int TetriminoL::getSize()
{
    return 3;
}

int TetriminoO::getSize()
{
    return 2;
}

int TetriminoS::getSize()
{
    return 3;
}

int TetriminoZ::getSize()
{
    return 3;
}

int TetriminoT::getSize()
{
    return 3;
}

map<Orientation, map<int, Pos>> TetriminoI::getRotationMap()
{
    return {
        {N,
         {{1, Pos(1, 1)},
          {2, Pos(1, 0)},
          {3, Pos(1, 3)},
          {4, Pos(1, 0)},
          {5, Pos(1, 3)}}},
        {W,
         {{1, Pos(1, 1)},
          {2, Pos(1, 1)},
          {3, Pos(1, 1)},
          {4, Pos(3, 1)},
          {5, Pos(0, 1)}}},
        {E,
         {{1, Pos(1, 1)},
          {2, Pos(1, 2)},
          {3, Pos(1, 2)},
          {4, Pos(0, 2)},
          {5, Pos(3, 2)}}},
        {S,
         {{1, Pos(1, 1)},
          {2, Pos(1, 3)},
          {3, Pos(1, 0)},
          {4, Pos(2, 3)},
          {5, Pos(2, 0)}}}};
}

map<Orientation, map<int, Pos>> TetriminoJ::getRotationMap()
{
    return {
        {N,
         {{1, Pos(1, 1)},
          {2, Pos(1, 1)},
          {3, Pos(1, 1)},
          {4, Pos(1, 1)},
          {5, Pos(1, 1)}}},
        {W,
         {{1, Pos(1, 1)},
          {2, Pos(1, 0)},
          {3, Pos(2, 0)},
          {4, Pos(-1, 1)},
          {5, Pos(-1, 0)}}},
        {E,
         {{1, Pos(1, 1)},
          {2, Pos(1, 2)},
          {3, Pos(2, 2)},
          {4, Pos(-1, 1)},
          {5, Pos(-1, 2)}}},
        {S,
         {{1, Pos(1, 1)},
          {2, Pos(1, 1)},
          {3, Pos(1, 1)},
          {4, Pos(1, 1)},
          {5, Pos(1, 1)}}}};
}

map<Orientation, map<int, Pos>> TetriminoL::getRotationMap()
{
    return {
        {N,
         {{1, Pos(1, 1)},
          {2, Pos(1, 1)},
          {3, Pos(1, 1)},
          {4, Pos(1, 1)},
          {5, Pos(1, 1)}}},
        {W,
         {{1, Pos(1, 1)},
          {2, Pos(1, 0)},
          {3, Pos(2, 0)},
          {4, Pos(-1, 1)},
          {5, Pos(-1, 0)}}},
        {E,
         {{1, Pos(1, 1)},
          {2, Pos(1, 2)},
          {3, Pos(2, 2)},
          {4, Pos(-1, 1)},
          {5, Pos(-1, 2)}}},
        {S,
         {{1, Pos(1, 1)},
          {2, Pos(1, 1)},
          {3, Pos(1, 1)},
          {4, Pos(1, 1)},
          {5, Pos(1, 1)}}}};
}

map<Orientation, map<int, Pos>> TetriminoO::getRotationMap()
{
    return {
        {N,
         {{1, Pos(1, 1)},
          {2, Pos(1, 1)},
          {3, Pos(1, 1)},
          {4, Pos(1, 1)},
          {5, Pos(1, 1)}}},
        {W,
         {{1, Pos(1, 1)},
          {2, Pos(1, 1)},
          {3, Pos(1, 1)},
          {4, Pos(1, 1)},
          {5, Pos(1, 1)}}},
        {E,
         {{1, Pos(1, 1)},
          {2, Pos(1, 1)},
          {3, Pos(1, 1)},
          {4, Pos(1, 1)},
          {5, Pos(1, 1)}}},
        {S,
         {{1, Pos(1, 1)},
          {2, Pos(1, 1)},
          {3, Pos(1, 1)},
          {4, Pos(1, 1)},
          {5, Pos(1, 1)}}}};
}

map<Orientation, map<int, Pos>> TetriminoS::getRotationMap()
{
    return {
        {N,
         {{1, Pos(1, 1)},
          {2, Pos(1, 1)},
          {3, Pos(1, 1)},
          {4, Pos(1, 1)},
          {5, Pos(1, 1)}}},
        {W,
         {{1, Pos(1, 1)},
          {2, Pos(1, 0)},
          {3, Pos(2, 0)},
          {4, Pos(-1, 1)},
          {5, Pos(-1, 0)}}},
        {E,
         {{1, Pos(1, 1)},
          {2, Pos(1, 2)},
          {3, Pos(2, 2)},
          {4, Pos(-1, 1)},
          {5, Pos(-1, 2)}}},
        {S,
         {{1, Pos(1, 1)},
          {2, Pos(1, 1)},
          {3, Pos(1, 1)},
          {4, Pos(1, 1)},
          {5, Pos(1, 1)}}}};
}

map<Orientation, map<int, Pos>> TetriminoT::getRotationMap()
{
    return {
        {N,
         {{1, Pos(1, 1)},
          {2, Pos(1, 1)},
          {3, Pos(1, 1)},
          {4, Pos(1, 1)},
          {5, Pos(1, 1)}}},
        {W,
         {{1, Pos(1, 1)},
          {2, Pos(1, 0)},
          {3, Pos(2, 0)},
          {4, Pos(-1, 1)},
          {5, Pos(-1, 0)}}},
        {E,
         {{1, Pos(1, 1)},
          {2, Pos(1, 2)},
          {3, Pos(2, 2)},
          {4, Pos(-1, 1)},
          {5, Pos(-1, 2)}}},
        {S,
         {{1, Pos(1, 1)},
          {2, Pos(1, 1)},
          {3, Pos(1, 1)},
          {4, Pos(1, 1)},
          {5, Pos(1, 1)}}}};
}

map<Orientation, map<int, Pos>> TetriminoZ::getRotationMap()
{
    return {
        {N,
         {{1, Pos(1, 1)},
          {2, Pos(1, 1)},
          {3, Pos(1, 1)},
          {4, Pos(1, 1)},
          {5, Pos(1, 1)}}},
        {W,
         {{1, Pos(1, 1)},
          {2, Pos(1, 0)},
          {3, Pos(2, 0)},
          {4, Pos(-1, 1)},
          {5, Pos(-1, 0)}}},
        {E,
         {{1, Pos(1, 1)},
          {2, Pos(1, 2)},
          {3, Pos(2, 2)},
          {4, Pos(-1, 1)},
          {5, Pos(-1, 2)}}},
        {S,
         {{1, Pos(1, 1)},
          {2, Pos(1, 1)},
          {3, Pos(1, 1)},
          {4, Pos(1, 1)},
          {5, Pos(1, 1)}}}};
}
