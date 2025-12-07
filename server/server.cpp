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

// int main(int argc, char *argv[])
// {
//     int sockfd, newsockfd, portno;
//     int client_id = 0;
//     socklen_t clilen;
//     struct sockaddr_in serv_addr, cli_addr;

//     if (argc < 2)
//     {
//         fprintf(stderr, "ERROR, no port provided\n");
//         exit(1);
//     }
//     sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     if (sockfd < 0)
//         error("ERROR opening socket");
//     bzero((char *)&serv_addr, sizeof(serv_addr));
//     portno = atoi(argv[1]);
//     serv_addr.sin_family = AF_INET;
//     serv_addr.sin_addr.s_addr = INADDR_ANY;
//     serv_addr.sin_port = htons(portno);
//     if (bind(sockfd, (struct sockaddr *)&serv_addr,
//              sizeof(serv_addr)) < 0)
//         error("ERROR on binding");
//     listen(sockfd, 5);
//     clilen = sizeof(cli_addr);
//     while (1)
//     {
//         cout << "ta aqui" << endl;
//         newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

//         client_id++;
//         client_sock_fds.push_back(newsockfd);

//         std::thread t1(dostuff, newsockfd, client_id);
//         t1.detach();
//     } /* end of while */
//     close(sockfd);
//     return 0; /* we never get here */
// }

// /******** DOSTUFF() *********************
//  There is a separate instance of this function
//  for each connection.  It handles all communication
//  once a connnection has been established.
//  *****************************************/
// void dostuff(int msock, int client_id)
// {
//     int n;
//     char buffer[256];
//     while (1)
//     {
//         bzero(buffer, 256);
//         cout << "chegou aqui" << endl;
//         n = read(msock, buffer, 255);
//         cout << "Received message from " << client_id << ": " << buffer << endl;
//         string send_str = to_string(client_id) + " says: ";
//         send_str += buffer;
//         for (auto &sock : client_sock_fds)
//         {
//             if (msock != sock)
//                 n = write(sock, send_str.c_str(), send_str.size());
//         }
//     }
// }