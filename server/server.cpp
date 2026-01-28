/* A simple server in the internet domain using TCP
   The port number is passed as an argument
   This version runs forever, forking off a separate
   process for each connection
*/
#include "server.hpp"

#define CODE_ROOM_FULL '0'
#define CODE_GAME_ONGOING '1'
#define CODE_ROOM_SIZE '2'
#define CODE_START_GAME '3'
#define CODE_ENEMY_MAP '4'

#define CODE_PLAYER_MAP '5'
#define CODE_PLAYER_DEAD '6'

using namespace std;
Server::Server(int port)
{
    up = false;
    my_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (my_socket < 0)
    {
        cout << "Error opening socket" << endl;
        return;
    }

    /* Configuring serv_address */
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    /* Binding serv_addr to server socket */
    if (bind(my_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        cout << "Error binding socket" << endl;
        return;
    }
    listen(my_socket, 5);
    clilen = sizeof(cli_addr);
    up = true;
}

bool Server::is_up()
{
    return up;
}

int Server::send(int to_client, char *msg, int msg_size)
{
    return write(to_client, msg, msg_size);
}

int Server::recv(int from_client, char *msg, int msg_size)
{
    return read(from_client, msg, msg_size);
}

int Server::wait_client()
{
    return accept(my_socket, (struct sockaddr *)&cli_addr, &clilen);
}

void Server::disconnect(int client)
{
    close(client);
}

void Server::disconnect()
{
    close(my_socket);
    up = false;
}

void dostuff(int, int); /* function prototype */
void error(const char *msg)
{
    perror(msg);
    exit(1);
}