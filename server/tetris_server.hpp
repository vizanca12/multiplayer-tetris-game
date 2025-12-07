#ifndef TETRIS_SERVER_HPP
#define TETRIS_SERVER_HPP

#include <thread>
#include <vector>
#include <mutex>
#include <map>

#include "server.hpp"
#include "player.hpp"
#include "room.hpp"

enum Status
{
    WAITING_PLAYERS,
    GAME_ONGOING
};

class TetrisServer
{
public:
    TetrisServer(int room_size, int server_port);
    void handle(Player *player);
    void broadcast(char code, char *message, int size);
    void broadcast_room_size();
    void change_watch();
    void init_watch();
    void handle_watch();
    void enter_room_phase();
    void enter_game_phase();

private:
    /*
        Store client socket file descriptors.
        They are used to send and receive messages to and from other users
    */
    Room *room;

    int players_alive = 0;

    Server *server;

    Status m_status;

    /*
        Mutex used to lock and unlock when doing critical work through threads
    */
    mutex mtx;
};

#endif