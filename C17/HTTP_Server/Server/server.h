#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

void close_socket(int socket, int isClientSocket);

/// Creates TCP/IP socket. -1 is returned if an error occurs, otherwise returns descriptor referencing the socket.
int HttpServer_create_socket(const char *ip, int port);

/// Starts listening to http requests via socket.
void HttpServer_start_listening(int serverSocket, int maxAllowedConnections);

#endif //HTTP_SERVER_H