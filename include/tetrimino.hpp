#ifndef TETRIS_TETRIMINO_HPP
#define TETRIS_TETRIMINO_HPP

#include <color.hpp>
#include <array>
#include <pos.hpp>
#include <map>
#include <SDL2/SDL.h>

using namespace std;

enum Orientation
{
    N, // 0
    W, // 1
    S, // 2
    E  // 3
};

enum TetriminoAction
{
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_DOWN,
    MOVE_UP,
    ROTATE_LEFT,
    ROTATE_RIGHT,
    DROP,
};

class Tetrimino
{
public:
    Tetrimino();

    // Rotates tetrimino using rot, and rotationPoint (number from 1 to 5)
    void rotateAction(TetriminoAction rot, int rotationPoint);

    // Moves tetrimino using mov
    void moveAction(TetriminoAction mov);

    // Undoes last movement or rotation
    void undoPreviousAction();

    // Reset tetriminos position and orientation
    void reset();

    // Returns minos positions
    virtual array<Pos, 4> getMinos() = 0;
    virtual int getSize() = 0;

    Pos &pos();

    const Pos &pos() const;

    char &name();
    const char &name() const;

    void setDead();

    // Draw tetrimino using SDL_Renderer
    void draw(SDL_Renderer *renderer);

    void draw(SDL_Renderer *renderer, int x, int y);

    static Tetrimino *generateTetriminoFrom(char c);

protected:
    char t_name;

    Pos m_pos;
    Pos prev_pos;

    Orientation m_orientation; // 0 for East, 1 for North, 2 for West, 3 for South
    Orientation prev_orientation;

    Color m_color;
    bool dead;

    // Returns rotation map
    virtual map<Orientation, map<int, Pos>> getRotationMap() = 0;
};

class TetriminoI : public Tetrimino
{
public:
    TetriminoI();
    virtual array<Pos, 4> getMinos();
    virtual map<Orientation, map<int, Pos>> getRotationMap();
    virtual int getSize();
};

class TetriminoJ : public Tetrimino
{
public:
    TetriminoJ();
    virtual array<Pos, 4> getMinos();
    virtual map<Orientation, map<int, Pos>> getRotationMap();
    virtual int getSize();
};

class TetriminoL : public Tetrimino
{
public:
    TetriminoL();
    virtual array<Pos, 4> getMinos();
    virtual map<Orientation, map<int, Pos>> getRotationMap();
    virtual int getSize();
};

class TetriminoO : public Tetrimino
{
public:
    TetriminoO();
    virtual array<Pos, 4> getMinos();
    virtual map<Orientation, map<int, Pos>> getRotationMap();
    virtual int getSize();
};

class TetriminoS : public Tetrimino
{
public:
    TetriminoS();
    virtual array<Pos, 4> getMinos();
    virtual map<Orientation, map<int, Pos>> getRotationMap();
    virtual int getSize();
};

class TetriminoT : public Tetrimino
{
public:
    TetriminoT();
    virtual array<Pos, 4> getMinos();
    virtual map<Orientation, map<int, Pos>> getRotationMap();
    virtual int getSize();
};

class TetriminoZ : public Tetrimino
{
public:
    TetriminoZ();
    virtual array<Pos, 4> getMinos();
    virtual map<Orientation, map<int, Pos>> getRotationMap();
    virtual int getSize();
};

#endif