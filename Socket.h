#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <string>
#include <cstring>

#include "../crossheader.h"

#if defined(_WIN32)
#include <conio.h>
#endif

class Socket
{
private:
    SOCKET socket_peer; // The socket file descriptor

public:
    Socket();
    ~Socket();

    bool create();
    bool bind(const std::string &localPort);
    bool connect(const std::string &remoteHost, const std::string &remotePort);
    void close();
    int send(const void *data, int lenght);
    bool listen(int backlog);
    Socket accept();
    int recv(void *buffer, int length);
    bool isValid() const; // Function to check if the socket is valid
    // Function to configure the local address
    std::pair<std::string, std::string> configureLocalAddress(const std::string &port);

    // Function to configure the remote address
    std::pair<std::string, std::string> configureRemoteAddress(const std::string &host, const std::string &port);
};

#endif // SOCKET_H
