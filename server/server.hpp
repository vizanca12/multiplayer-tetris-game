#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

class Server
{
public:
    Server(int port);
    int send(int to_client, char *msg, int msg_size);
    int recv(int from_client, char *msg, int msg_size);
    int wait_client();
    void disconnect(int client);
    void disconnect();
    bool is_up();

private:
    /* Socket from server*/
    int my_socket;

    /* Other stuff to make the communication */
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    /* */
    bool up;
};

#endif