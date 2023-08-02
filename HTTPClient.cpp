#include "HTTPClient.h"
#include <iostream>

HTTPClient::HTTPClient() {}

HTTPClient::~HTTPClient() {
    clientSocket.close();
}

bool HTTPClient::connectToServer(const std::string& host, const std::string& port) {
    if (!clientSocket.create()) {
        std::cerr << "Failed to create the client socket.\n";
        return false;
    }

    if (!clientSocket.connect(host, port)) {
        std::cerr << "Failed to connect to the HTTP server.\n";
        return false;
    }

    return true;
}

void HTTPClient::performGETRequest() {
    const std::string GET_REQUEST = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n";

    if (clientSocket.send(GET_REQUEST.c_str(), GET_REQUEST.size()) == -1) {
        std::cerr << "Error sending data to the server.\n";
        return;
    }

    char response[1024];
    int bytesReceived = clientSocket.recv(response, sizeof(response) - 1);
    if (bytesReceived <= 0) {
        std::cerr << "Error while receiving data from the server.\n";
        return;
    }
    response[bytesReceived] = '\0';
    std::cout << "Received response from server:\n" << response << std::endl;
}

