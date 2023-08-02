#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "Socket.h"
#include <string>

class HTTPServer {
private:
    Socket serverSocket;
    const std::string HTTP_RESPONSE = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 14\r\n\r\nHTTP response\r\n";

public:
    HTTPServer();
    ~HTTPServer();

    bool startServer(const std::string& port);
    void handleClientRequest();
};

#endif // HTTPSERVER_H

