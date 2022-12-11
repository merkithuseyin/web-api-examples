#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "server.h"
#include "../Mapper/mapper.h"

#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void close_socket(int socket, int isClientSocket){
    int isSuccess = close(socket) == 0;

    if      ( isSuccess &&  isClientSocket) puts("Client socket closed");
    else if ( isSuccess && !isClientSocket) puts("Server socket closed");
    else if (!isSuccess &&  isClientSocket) perror("Closing client socket failed");
    else if (!isSuccess && !isClientSocket) perror("Closing server socket failed");
}

int HttpServer_create_socket(const char *ip, int port){
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        fprintf(stderr, "Couldn't create socket\r\n");
        return -1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ip);
    if(bind(sock, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) < 0){
        perror("Binding failed");
        close_socket(sock, 0);
        return -1;
    }

    printf("Bound: %s:%d...\r\n", inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));
    return sock;
}

void HttpServer_start_listening(int serverSocket, int maxAllowedConnections){
    while(1){
        if(listen(serverSocket, maxAllowedConnections) < 0){
            perror("Listening for connections failed");
            close_socket(serverSocket, 0);
            break;
        }

        puts("Listening...");

        struct sockaddr_in clientAddress;
        int clientAddressSize = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr*) &clientAddress, (socklen_t*) &clientAddressSize);
        if (clientSocket < 0){
            perror("Accepting connection failed");
            close_socket(clientSocket, 1);
            continue;
        }

        char clientMessage[1000];
        memset(clientMessage, '\0', sizeof(clientMessage));
        long receivedBytesCount = recv(clientSocket, clientMessage, sizeof(clientMessage), 0);
        if (receivedBytesCount < 0){
            fprintf(stderr, "Receiving message failed\r\n");
            close_socket(clientSocket, 1);
            continue;
        }
        if (receivedBytesCount == 0){
            puts("No message received, the peer has closed its half side of the connection.");
            close_socket(clientSocket, 1);
            continue;
        }

        printf(ANSI_COLOR_GREEN "Message received from %s:%d :\r\n" ANSI_COLOR_BLUE "%s\r\n\r\n" ANSI_COLOR_RESET ,
               inet_ntoa(clientAddress.sin_addr),
               ntohs(clientAddress.sin_port),
               clientMessage);

        char *serverResponseStr;
        HttpMapper_handle_request(clientMessage, &serverResponseStr);

        if (send(clientSocket, serverResponseStr, strlen(serverResponseStr), 0) < 0){
            perror("Responding to client failed");
            close_socket(clientSocket, 1);
            free(serverResponseStr);
            continue;
        }

        printf(ANSI_COLOR_GREEN "Responded to client %s:%d :\r\n" ANSI_COLOR_BLUE "%s\r\n\r\n" ANSI_COLOR_RESET,
               inet_ntoa(clientAddress.sin_addr),
               ntohs(clientAddress.sin_port),
               serverResponseStr
        );

        close_socket(clientSocket, 1);
        free(serverResponseStr);
    }
}