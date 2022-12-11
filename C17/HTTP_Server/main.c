#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Server/server.h"

#define LOCALHOST "127.0.0.1"

int g_serverSocket = -1;
static void signalHandler(){
    close_socket(g_serverSocket, 0);
}

static unsigned short get_port(int argc, const char **argv){
    for (int i = 0; i < argc; ++i) {
        if((strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--port") == 0) && (i + 1 < argc)){
            return strtonum(argv[i + 1], 1, 65535, NULL);
        }
    }
    return 0;
}

int main(int argc, char **argv) {
    puts("Application started...");

    g_serverSocket = HttpServer_create_socket(LOCALHOST, get_port(argc, (const char **) argv));
    if (g_serverSocket < 0) {
        return -1;
    }

    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    HttpServer_start_listening(g_serverSocket, 10);

    puts("Application stopped!");
    return 0;
}