#ifndef TETRIS_MAP_HPP
#define TETRIS_MAP_HPP

#include <pos.hpp>
#include <tetrimino.hpp>
#include <globals.hpp>
#include <vector>
#include <mino.hpp>

#include "text.hpp"

#include <SDL2/SDL.h>

enum TetrisPhase
{
    GENERATION_PHASE,
    FALLING_PHASE,
    LOCK_PHASE,
    PATTERN_PHASE,
    ANIMATION_PHASE,
    ELIMINATE_PHASE,
};

class TetrisMap
{
public:
    TetrisMap(bool extended_map);
    Mino at(Pos pos);
    Mino at(int row, int col);

    void set(int row, int col, Mino m);
    void set(Pos pos, Mino m);
    void draw(SDL_Renderer *renderer);
    void tetriminoAction(TetriminoAction action);

    void getMap(char *m_map);
    char *enemyMap();

    bool isGameOver();

    // Tick
    void tick();

    void lockTetrimino();

    void generateNextTetrimino();

    void changeHold();

    void copyRow(int row_source, int row_destiny);

    void getGameStatus(int *score, int *lines, int *level);

    void addBufferLines(int lines);
    int getBufferLines();
    void resetBufferLines();

private:
    // Matrix Height + 2 (Skyline) + 2 (Top wall) + 2 (Bottom wall)
    // Matrix Width + 2 (Left wall) + 2 (Right wall)
    // char can be: O, I, T, L, J, S, Z, W (for wall)
    Mino m_matrix[MATRIX_HEIGHT + 2 + 2 + 2][MATRIX_WIDTH + 2 + 2];

    Tetrimino *tetrimino;

    Text *text;

    bool extended_map;

    char enemy_map[MATRIX_WIDTH * MATRIX_HEIGHT];

    int bufferLines = 0;
    int lines_cleared = 0;
    int level = 0;
    int score = 0;
    int fall_speed = 1000; // milliseconds
    int t_down_time = fall_speed;

    TetrisPhase phase;

    vector<int> rowsDestroyed;
    vector<char> virtualBag = {'I', 'O', 'T', 'L', 'J', 'Z', 'S'};
    vector<Tetrimino *> tetriminoQueue;

    Tetrimino *tetriminoHold = nullptr;
    bool canChangeHold = true;

    // Undoes tetriminoAction if it is colliding with something, returns true if it was validated.
    bool validateTetriminoAction();
    bool tetriminoCollides();
    void initTetriminoQueue();
    bool canFall();
    void applyBufEnemyLines();

    void drawGameStatus(SDL_Renderer *renderer);
    void drawGhostMinos(SDL_Renderer *renderer);
    void drawQueueTetriminos(SDL_Renderer *renderer);
    void drawTetriminoHold(SDL_Renderer *renderer);
    void drawEnemyMap(SDL_Renderer *renderer);
    void drawGameOver(SDL_Renderer *renderer);
    void drawBufEnemyLines(SDL_Renderer *renderer);

    /// @brief Update variables such as level, score and line cleared and fall_speed
    /// The game status are represented by these variables
    /// @param nbLinesCleared
    void updateGameStatus(int nbLinesCleared);

    int lockDownTimer;

    bool gameOver = false;
};

#endif
