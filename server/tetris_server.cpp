#include "tetris_server.hpp"
#include <ctime>
#include <globals.hpp>

TetrisServer::TetrisServer(int room_size, int server_port)
{
    room = new Room(room_size);
    server = new Server(server_port);

    // Check if it was able to initialize the server;
    if (!server->is_up())
        return;
    else
        std::cout << "Server is up!" << std::endl;

    enter_room_phase();

    while (1)
    {
        int new_client = server->wait_client();

        if (room->is_filled() || m_status == GAME_ONGOING)
        {
            std::cout << "New client tried to connect, but there's no room available..." << std::endl;
            char message = CODE_ROOM_UNAVAILABLE;
            server->send(new_client, &message, 1);
            server->disconnect(new_client);
        }

        std::cout << "New client entered the room!" << std::endl;
        Player *new_player = new Player(new_client);
        room->add(new_player);

        broadcast_room_size();

        // All players arrived to the server!
        if (room->is_filled())
            enter_game_phase();

        std::thread t1(&TetrisServer::handle, this, new_player);
        t1.detach();
    } /* end of while */
    server->disconnect();
}

void TetrisServer::enter_room_phase()
{
    std::cout << "Entered room phase, waiting for other players!" << std::endl;
    m_status = WAITING_PLAYERS;
    players_alive = room->max_size();
}

void TetrisServer::enter_game_phase()
{
    std::cout << "Entered game phase!" << std::endl;
    this->broadcast(CODE_START_GAME, NULL, 0);
    m_status = GAME_ONGOING;
    std::thread t_watch(&TetrisServer::handle_watch, this);
    t_watch.detach();
}

void TetrisServer::broadcast_room_size()
{
    // Send to everyone that someone new player have just got in!
    char frac[3];
    room->get_frac_filled(frac);
    broadcast(CODE_ROOM_SIZE, frac, 3);
}

void TetrisServer::init_watch()
{
    for (int i = 0; i < room->current_size(); i++)
    {
        Player *player = room->get_player(i);
        player->set_id(i);

        Player *watching = room->get_player(i + 1);
        player->set_watching(i + 1);
        watching->set_watched_by(i);
    }
}

void TetrisServer::handle_watch()
{
    init_watch();

    while (m_status == GAME_ONGOING)
    {
        std::time_t start_ms = std::time(nullptr);
        std::time_t diff = 0;
        while (diff < 10)
        {
            diff = std::time(nullptr) - start_ms;
        }
        change_watch();
    }
}

void TetrisServer::change_watch()
{
    for (int i = 0; i < room->current_size() && players_alive > 1; i++)
    {
        Player *player = room->get_player(i);
        int i_watching = player->get_watching() + 1;
        Player *watching = room->get_player(i_watching);

        // New player to watch must be alive and we cannot watch ourselves
        while (!watching->is_alive() || watching->get_id() == player->get_id())
        {
            i_watching = i_watching + 1;
            watching = room->get_player(i_watching);
        }

        player->set_watching(i_watching);
        watching->set_watched_by(i);
    }
}

void TetrisServer::handle(Player *player)
{
    char tetris_map[200];
    char code;
    bool connected = true;
    int res = 1;

    while (connected)
    {
        // Get code sent by client, can be CODE_PLAYER_MAP or CODE_PLAYER_DEAD
        res = server->recv(player->get_socket(), &code, 1);

        if (code == CODE_PLAYER_MAP)
        {
            int i_watched_by = player->get_watched_by();
            Player *to_send = room->get_player(i_watched_by);
            server->recv(player->get_socket(), tetris_map, 200);
            server->send(to_send->get_socket(), &code, 1);
            server->send(to_send->get_socket(), tetris_map, 200);
        }
        else if (code == CODE_PLAYER_LINES)
        {
            int i_watching = player->get_watching();
            Player *to_send = room->get_player(i_watching);
            char linesToSend = 0;
            server->recv(player->get_socket(), &linesToSend, 1);
            server->send(to_send->get_socket(), &code, 1);
            server->send(to_send->get_socket(), &linesToSend, 1);
        }
        else if (code == CODE_PLAYER_DEAD)
        {
            player->set_alive(false);
            players_alive--;
            std::cout << "Player died " << player->get_id() << std::endl;

            if (players_alive == 1)
                broadcast(CODE_GAME_OVER, NULL, 0);
        }
        else if (code == CODE_PLAYER_DISCONNECT)
        {
            connected = false;
        }

        connected &= (res > 0);
    }

    mtx.lock();

    std::cout << "Player disconnected " << player->get_socket() << std::endl;
    room->remove(player);
    
    // Adicione esta pequena pausa antes de desconectar
    // This gives time for "Game Over" to reach the client
    usleep(100000); // 100ms
    
    server->disconnect(player->get_socket());

    mtx.unlock();

    // Broadcast to everyone the new room size (because a player exited)
    if (m_status == WAITING_PLAYERS)
        broadcast_room_size();

    if (room->is_empty())
        enter_room_phase();

    // Player forcely disconnected
    if (m_status == GAME_ONGOING)
    {
        player->set_alive(false);
        players_alive--;

        if (players_alive == 1)
            broadcast(CODE_GAME_OVER, NULL, 0);
        else
            init_watch();
    }
   
}

void TetrisServer::broadcast(char code, char *message, int msg_size)
{
    for (int i = 0; i < room->current_size(); i++)
    {
        int socket = room->get_player(i)->get_socket();
        server->send(socket, &code, 1);
        if (msg_size)
            server->send(socket, message, msg_size);
    }
}