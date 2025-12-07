#ifndef TETRIS_PLAYER_HPP
#define TETRIS_PLAYER_HPP
#include <string>

using namespace std;

class Player
{
public:
    Player(int sock);
    void set_watching(int);
    void set_watched_by(int);
    int get_watching();
    int get_watched_by();
    int get_socket();
    bool is_alive();
    void set_alive(bool alive);
    void set_id(int);
    int get_id();

private:
    // Represents idx of player in the room whom I am watching
    int i_watching;

    // Represents idx of player that is watching me;
    int i_watched_by;

    bool alive;
    int socket;
    int id;
};

#endif