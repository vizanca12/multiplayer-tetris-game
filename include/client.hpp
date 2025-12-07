#ifndef CLIENT_TETRIS_HPP
#define CLIENT_TETRIS_HPP

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

class Client
{
public:
    Client(const char *hostname, int port);
    int recv(char *msg, int msg_size);
    int send(char *msg, int msg_size);
    bool is_connected();
    void disconnect();

private:
    bool connected;
    int m_socket;
    struct sockaddr_in serv_addr;
    struct hostent *server;
};

#endif