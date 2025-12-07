#include "tetris_server.hpp"
#include "globals.hpp"
#include <string>

int main(int argc, char *argv[])
{
    int numPlayers = std::stoi(argv[1]);
    TetrisServer ts(numPlayers, SERVER_PORT);
    return 0;
}