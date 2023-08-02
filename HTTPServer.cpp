#include "HTTPServer.h"
#include <iostream>

HTTPServer::HTTPServer() {}

HTTPServer::~HTTPServer() {
    serverSocket.close();
}

bool HTTPServer::startServer(const std::string& port) {
    if (!serverSocket.create() || !serverSocket.bind(port) || !serverSocket.listen(2)) {
        std::cerr << "Failed to start the HTTP server.\n";
        return false;
    }

    std::cout << "Server is listening on port " << port << "...\n";
    return true;
}

void HTTPServer::handleClientRequest() {
    while (true) {
        Socket clientSocket = serverSocket.accept();
        if (!clientSocket.isValid()) {
            std::cerr << "Failed to accept the incoming connection.\n";
            continue;
        }

        char request[1024];
        int bytesReceived = clientSocket.recv(request, sizeof(request) - 1);
        if (bytesReceived <= 0) {
            std::cerr << "Error while receiving data from the client.\n";
            continue;
        }

        request[bytesReceived] = '\0';
        std::cout << "Received request from client:\n" << request << std::endl;

        clientSocket.send(HTTP_RESPONSE.c_str(), HTTP_RESPONSE.size());
    }
}

