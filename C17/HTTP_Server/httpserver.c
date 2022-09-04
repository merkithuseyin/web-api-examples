#include "httpserver.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int g_serverSocket;

void HttpServer_StartListening(unsigned short port, const int *isCancelled){
    char ipString[] = "127.0.0.1";

    if (port <= 0 || port > 65535){
        perror("Port is out of range!");
        exit(EXIT_FAILURE);
    }

    g_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(g_serverSocket < 0){
        perror("Couldn't create socket!");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ipString);

    if(bind(g_serverSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) < 0){
        perror("Couldn't bind to the port!");
        exit(EXIT_FAILURE);
    }

    while(!*isCancelled){

        // Listen for clients
        if(listen(g_serverSocket, 1) < 0){
            perror("Error while listening!");
            close(g_serverSocket);
            exit(EXIT_FAILURE);
        }
        printf("\nListening for incoming connections on %s:%i...\n", ipString, port);

        // Accept an incoming connection
        struct sockaddr_in clientAddress;
        int clientSize = sizeof(clientAddress);
        int clientSocket = accept(g_serverSocket, (struct sockaddr*) &clientAddress, (socklen_t*) &clientSize);

        if (clientSocket < 0){
            perror("Couldn't accept connection!");
            close(clientSocket);
            close(g_serverSocket);

            if (*isCancelled)
                exit(EXIT_SUCCESS);
            else
                exit(EXIT_FAILURE);
        }

        // Receive client's message
        char client_message[1000];
        memset(client_message, '\0', sizeof(client_message));

        if (recv(clientSocket, client_message, sizeof(client_message), 0) < 0){
            perror("Couldn't receive message!");
            close(clientSocket);
            close(g_serverSocket);
            exit(EXIT_FAILURE);
        }

        printf("Message received from %s:%i :\n" ANSI_COLOR_GREEN "%s\n" ANSI_COLOR_RESET ,
               inet_ntoa(clientAddress.sin_addr),
               ntohs(clientAddress.sin_port),
               client_message);

        // string apiResponse = HandleRequest(); //todo: 'client_message' is expected to be in http. handle it and give answer to client!

        // Respond to client
        char server_message[100];
        memset(server_message, '\0', sizeof(server_message));
        strcpy(server_message, "This is the server's message.");

        if (send(clientSocket, server_message, strlen(server_message), 0) < 0){
            perror("Couldn't respond!");
            close(clientSocket);
            close(g_serverSocket);
            exit(EXIT_FAILURE);
        }

        printf("Responded to client %s:%i :\n" ANSI_COLOR_BLUE "%s\n\n" ANSI_COLOR_RESET,
               inet_ntoa(clientAddress.sin_addr),
               ntohs(clientAddress.sin_port),
               server_message
        );

        close(clientSocket);
        puts("Client socket closed!");
    }

    close(g_serverSocket);
    puts("Server socket closed!\n");
}

void HttpServer_StopListening(){
    puts("Signal received!");
    sleep(1); // just in case, wait for finishing conversation with client instead of ghosting
    puts("Stopping HttpServer...");
    close(g_serverSocket);
}