#include "HTTPServer.h"

int main() {
    HTTPServer server;
    if (!server.startServer("8080")) {
        return 1;
    }

    server.handleClientRequest();

    return 0;
}

