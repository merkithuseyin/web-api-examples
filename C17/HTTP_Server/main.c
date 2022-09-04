#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "httpserver.h"

int g_signalReceived = 0;

static void signalHandler()
{
    g_signalReceived = 1;
    HttpServer_StopListening();
}

unsigned short GetPort(int argc, char *argv[]){
    for (int i = 0; i < argc; ++i) {
        if((strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--port") == 0) && (i + 1 < argc)){
            return strtonum(argv[i + 1], 1, 65535, NULL);
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    puts("Application started...");

    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    HttpServer_StartListening(GetPort(argc, argv), &g_signalReceived);

    puts("Application stopped!");
    return 0;
}