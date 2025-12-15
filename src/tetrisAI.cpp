#include <tetrisAI.hpp>
#include <cmath>
#include <climits>
#include <iostream>
#include <SDL2/SDL.h>
#include <cstdlib> // Include for rand() and srand()

TetrisAI::TetrisAI(TetrisMap *map) : tetrisMap(map)
{
    lastMoveTime = SDL_GetTicks();
    srand(SDL_GetTicks()); // Seed random number generator with current time
}

void TetrisAI::setSpeed(int delayMs)
{
    moveDelay = delayMs;
}

void TetrisAI::makeMove()
{
    if (tetrisMap->isGameOver())
        return;

    int currentTime = SDL_GetTicks();
    if (currentTime - lastMoveTime < moveDelay)
        return;

    lastMoveTime = currentTime;

    // This is safer and avoids complex state tracking issues
    
    // Random decision: rotate, move, or drop
    int action = rand() % 100;
    
    if (action < 10)
    {
        // 10% chance to rotate
        tetrisMap->tetriminoAction(ROTATE_RIGHT);
    }
    else if (action < 40)
    {
        // 30% chance to move left
        tetrisMap->tetriminoAction(MOVE_LEFT);
    }
    else if (action < 70)
    {
        // 30% chance to move right
        tetrisMap->tetriminoAction(MOVE_RIGHT);
    }
    else if (action < 90)
    {
        // 20% chance to move down faster
        tetrisMap->tetriminoAction(MOVE_DOWN);
    }
    else
    {
        // 10% chance to drop
        tetrisMap->tetriminoAction(DROP);
    }
}

TetrisAI::MoveEvaluation TetrisAI::findBestMove()
{
    MoveEvaluation best;
    best.score = -999999;
    best.targetCol = MATRIX_WIDTH / 2;
    best.targetRotation = 0;

    // Obtém o estado atual do tabuleiro
    char currentMap[MATRIX_WIDTH * MATRIX_HEIGHT];
    tetrisMap->getMap(currentMap);

    // Converte para matriz 2D para facilitar manipulação
    char board[MATRIX_HEIGHT][MATRIX_WIDTH];
    for (int row = 0; row < MATRIX_HEIGHT; row++)
    {
        for (int col = 0; col < MATRIX_WIDTH; col++)
        {
            board[row][col] = currentMap[row * MATRIX_WIDTH + col];
        }
    }

    // Limpa as posições da peça atual do tabuleiro para simulação
    for (int row = 0; row < MATRIX_HEIGHT; row++)
    {
        for (int col = 0; col < MATRIX_WIDTH; col++)
        {
            char c = board[row][col];
            if (c != 0 && c != 'W')
            {
                // Verifica se é parte da peça atual (nas primeiras linhas)
                if (row < 4)
                    board[row][col] = 0;
            }
        }
    }

    // Testa todas as rotações (0-3) e posições horizontais
    for (int rotation = 0; rotation < 4; rotation++)
    {
        for (int col = 0; col < MATRIX_WIDTH; col++)
        {
            // Cria uma cópia do tabuleiro para simulação
            char testBoard[MATRIX_HEIGHT][MATRIX_WIDTH];
            for (int r = 0; r < MATRIX_HEIGHT; r++)
            {
                for (int c = 0; c < MATRIX_WIDTH; c++)
                {
                    testBoard[r][c] = board[r][c];
                }
            }

            // Simula a colocação da peça
            // Para simplificar, usamos uma simulação básica
            int dropRow = 0;
            bool canPlace = true;

            // Encontra onde a peça cairia nesta coluna
            for (int row = 0; row < MATRIX_HEIGHT; row++)
            {
                bool blocked = false;
                // Verifica se há espaço (simplificado - assume peça 2x2)
                for (int dc = 0; dc < 2 && col + dc < MATRIX_WIDTH; dc++)
                {
                    if (testBoard[row][col + dc] != 0)
                    {
                        blocked = true;
                        break;
                    }
                }
                if (blocked)
                {
                    dropRow = row - 1;
                    break;
                }
                dropRow = row;
            }

            if (dropRow < 0)
                canPlace = false;

            if (canPlace && col < MATRIX_WIDTH - 1)
            {
                // Coloca a peça no tabuleiro de teste
                for (int dc = 0; dc < 2 && col + dc < MATRIX_WIDTH; dc++)
                {
                    for (int dr = 0; dr < 2 && dropRow - dr >= 0; dr++)
                    {
                        testBoard[dropRow - dr][col + dc] = 'X';
                    }
                }

                // Avalia a posição resultante
                double score = evaluatePosition(testBoard);

                if (score > best.score)
                {
                    best.score = score;
                    best.targetCol = col;
                    best.targetRotation = rotation;
                }
            }
        }
    }

    return best;
}

double TetrisAI::evaluatePosition(char board[MATRIX_HEIGHT][MATRIX_WIDTH])
{
    // Pesos para diferentes fatores (ajustados para uma IA competente)
    const double WEIGHT_HEIGHT = -0.510066;
    const double WEIGHT_LINES = 0.760666;
    const double WEIGHT_HOLES = -0.35663;
    const double WEIGHT_BUMPINESS = -0.184483;

    int aggregateHeight = getAggregateHeight(board);
    int completeLines = getCompleteLines(board);
    int holes = getHoles(board);
    int bumpiness = getBumpiness(board);

    return WEIGHT_HEIGHT * aggregateHeight +
           WEIGHT_LINES * completeLines +
           WEIGHT_HOLES * holes +
           WEIGHT_BUMPINESS * bumpiness;
}

int TetrisAI::getAggregateHeight(char board[MATRIX_HEIGHT][MATRIX_WIDTH])
{
    int total = 0;
    for (int col = 0; col < MATRIX_WIDTH; col++)
    {
        total += getColumnHeight(board, col);
    }
    return total;
}

int TetrisAI::getColumnHeight(char board[MATRIX_HEIGHT][MATRIX_WIDTH], int col)
{
    for (int row = 0; row < MATRIX_HEIGHT; row++)
    {
        if (board[row][col] != 0)
        {
            return MATRIX_HEIGHT - row;
        }
    }
    return 0;
}

int TetrisAI::getCompleteLines(char board[MATRIX_HEIGHT][MATRIX_WIDTH])
{
    int lines = 0;
    for (int row = 0; row < MATRIX_HEIGHT; row++)
    {
        bool complete = true;
        for (int col = 0; col < MATRIX_WIDTH; col++)
        {
            if (board[row][col] == 0)
            {
                complete = false;
                break;
            }
        }
        if (complete)
            lines++;
    }
    return lines;
}

int TetrisAI::getHoles(char board[MATRIX_HEIGHT][MATRIX_WIDTH])
{
    int holes = 0;
    for (int col = 0; col < MATRIX_WIDTH; col++)
    {
        bool foundBlock = false;
        for (int row = 0; row < MATRIX_HEIGHT; row++)
        {
            if (board[row][col] != 0)
            {
                foundBlock = true;
            }
            else if (foundBlock)
            {
                holes++;
            }
        }
    }
    return holes;
}

int TetrisAI::getBumpiness(char board[MATRIX_HEIGHT][MATRIX_WIDTH])
{
    int bumpiness = 0;
    int prevHeight = getColumnHeight(board, 0);
    
    for (int col = 1; col < MATRIX_WIDTH; col++)
    {
        int height = getColumnHeight(board, col);
        bumpiness += abs(height - prevHeight);
        prevHeight = height;
    }
    return bumpiness;
}
