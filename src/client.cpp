#include "client.hpp"
#include <iostream>
#include <thread>

using namespace std;

Client::Client(const char *hostname, int port)
{
    connected = false;
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket < 0)
    {
        std::cout << "Error opening socket" << std::endl;
        return;
    }
    server = gethostbyname(hostname);
    if (server == NULL)
    {
        std::cout << "Error connecting to server with hostname: " << hostname << std::endl;
        return;
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(port);
    if (connect(m_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cout << "ERROR connecting" << std::endl;
        return;
    }

    connected = true;
}

bool Client::is_connected()
{
    return connected;
}

// Receive from server a message;
// Returns 0 (EOF), -1 (Error) or 1 (Read correctly) and value is in code;
int Client::recv(char *msg, int msg_size)
{
    return read(m_socket, msg, msg_size);
}

int Client::send(char *msg, int msg_size)
{
    return write(m_socket, msg, msg_size);
}

void Client::disconnect()
{
    if (connected)
        close(m_socket);

    connected = false;
}