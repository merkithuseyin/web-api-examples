#ifndef HTTP_SERVER_HTTPSERVER_H
#define HTTP_SERVER_HTTPSERVER_H

///Starts listening to specified port
void HttpServer_StartListening(unsigned short port, const int *isCancelled);
void HttpServer_StopListening();

#endif //HTTP_SERVER_HTTPSERVER_H