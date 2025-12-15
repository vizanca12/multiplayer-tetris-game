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

    // Calcula e executa o melhor movimento para a peça atual
    void makeMove();

    // Define a velocidade da IA (delay entre movimentos em ms)
    void setSpeed(int delayMs);

private:
    TetrisMap *tetrisMap;
    int moveDelay = 50; // Delay entre movimentos em ms
    int lastMoveTime = 0;

    // Estrutura para armazenar uma posição avaliada
    struct MoveEvaluation
    {
        int targetCol;
        int targetRotation;
        double score;
    };

    // Avalia uma posição específica do tabuleiro
    double evaluatePosition(char board[MATRIX_HEIGHT][MATRIX_WIDTH]);

    // Simula colocar uma peça em uma posição
    void simulatePlacement(char board[MATRIX_HEIGHT][MATRIX_WIDTH], 
                           Tetrimino *piece, int col, int rotation);

    // Encontra a melhor jogada para a peça atual
    MoveEvaluation findBestMove();

    // Calcula a altura agregada do tabuleiro
    int getAggregateHeight(char board[MATRIX_HEIGHT][MATRIX_WIDTH]);

    // Calcula o número de linhas completas
    int getCompleteLines(char board[MATRIX_HEIGHT][MATRIX_WIDTH]);

    // Calcula o número de buracos
    int getHoles(char board[MATRIX_HEIGHT][MATRIX_WIDTH]);

    // Calcula a irregularidade (diferença entre colunas adjacentes)
    int getBumpiness(char board[MATRIX_HEIGHT][MATRIX_WIDTH]);

    // Obtém a altura de uma coluna específica
    int getColumnHeight(char board[MATRIX_HEIGHT][MATRIX_WIDTH], int col);

    // Estado atual da IA
    bool hasTarget = false;
    int targetCol;
    int targetRotation;
    int currentRotation = 0;
};

#endif
