#ifndef TETRIS_ROOM_HPP
#define TETRIS_ROOM_HPP

#include <vector>
#include "player.hpp"
using namespace std;

class Room
{
public:
    Room(int size);
    bool is_filled();
    bool is_empty();
    void add(Player *player);
    void remove(Player *player);
    int current_size();
    int max_size();
    void get_frac_filled(char *frac);
    Player *get_player(int i);

private:
    int m_size;
    vector<Player *> players;
};

#endif