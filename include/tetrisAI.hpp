#ifndef TETRIS_AI_HPP
#define TETRIS_AI_HPP

#include <tetrisMap.hpp>
#include <tetrimino.hpp>
#include <globals.hpp>
#include <array>
#include <vector>

using namespace std;

// AI que joga Tetris automaticamente
class TetrisAI
{
public:
    TetrisAI(TetrisMap *map);

    // Calculate and execute the best move for the current piece
    void makeMove();

    // Define the AI speed (delay between moves in ms)
    void setSpeed(int delayMs);

private:
    TetrisMap *tetrisMap;
    int moveDelay = 50; // Delay between moves in ms
    int lastMoveTime = 0;

    // Structure to store an evaluated position
    struct MoveEvaluation
    {
        int targetCol;
        int targetRotation;
        double score;
    };

    // Evaluate a specific board position
    double evaluatePosition(char board[MATRIX_HEIGHT][MATRIX_WIDTH]);

    // Simulate placing a piece at a position
    void simulatePlacement(char board[MATRIX_HEIGHT][MATRIX_WIDTH], 
                           Tetrimino *piece, int col, int rotation);

    // Find the best move for the current piece
    MoveEvaluation findBestMove();

    // Calcula a altura agregada do tabuleiro
    int getAggregateHeight(char board[MATRIX_HEIGHT][MATRIX_WIDTH]);

    // Calculate the number of completed lines
    int getCompleteLines(char board[MATRIX_HEIGHT][MATRIX_WIDTH]);

    // Calculate the number of holes
    int getHoles(char board[MATRIX_HEIGHT][MATRIX_WIDTH]);

    // Calculate roughness (difference between adjacent columns)
    int getBumpiness(char board[MATRIX_HEIGHT][MATRIX_WIDTH]);

    // Get the height of a specific column
    int getColumnHeight(char board[MATRIX_HEIGHT][MATRIX_WIDTH], int col);

    // Estado atual da IA
    bool hasTarget = false;
    int targetCol;
    int targetRotation;
    int currentRotation = 0;
};

#endif
