#include "Socket.h"

Socket::Socket() : socket_peer(-1)
{
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
}

Socket::~Socket()
{
    close();
#ifdef _WIN32
    WSACleanup();
#endif
}

bool Socket::create()
{
    printf("Creating socket...\n");

    // Create the socket
    socket_peer = socket(AF_INET, SOCK_STREAM, 0);
    return ISVALIDSOCKET(socket_peer);
}

bool Socket::bind(const std::string &localPort)
{
    printf("Binding socket...\n");
    std::pair<std::string, std::string> localAddress = configureLocalAddress(localPort);
    std::string localAddressStr = localAddress.first;
    std::string localPortStr = localAddress.second;

    // Fill in the socket address structure with the local address details
    struct sockaddr_in localSockAddr;
    memset(&localSockAddr, 0, sizeof(localSockAddr));
    localSockAddr.sin_family = AF_INET;
    inet_pton(AF_INET, localAddressStr.c_str(), &(localSockAddr.sin_addr));
    localSockAddr.sin_port = htons(std::stoi(localPortStr));

    // Bind the socket to the local address and port
    if (::bind(socket_peer, (struct sockaddr *)&localSockAddr, sizeof(localSockAddr)) == -1)
    {
        fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
        return false;
    }

    // The binding was successful
    return true;
}

bool Socket::connect(const std::string &remoteHost, const std::string &remotePort)
{
    printf("Connecting socket...\n");
    std::pair<std::string, std::string> remoteAddress = configureRemoteAddress(remoteHost, remotePort);
    std::string remoteAddressStr = remoteAddress.first;
    std::string remotePortStr = remoteAddress.second;

    // Fill in the socket address structure with the remote address details
    struct sockaddr_in remoteSockAddr;
    memset(&remoteSockAddr, 0, sizeof(remoteSockAddr));
    remoteSockAddr.sin_family = AF_INET;
    inet_pton(AF_INET, remoteAddressStr.c_str(), &(remoteSockAddr.sin_addr));
    remoteSockAddr.sin_port = htons(std::stoi(remotePortStr));

    // Connect the socket to the remote address and port
    if (::connect(socket_peer, (struct sockaddr *)&remoteSockAddr, sizeof(remoteSockAddr)) == -1)
    {
        fprintf(stderr, "connect() failed. (%d)\n", GETSOCKETERRNO());
        return false;
    }

    // The connection was successful
    return true;
}

void Socket::close()
{
    printf("Closing socket...\n");
#ifdef _WIN32
    closesocket(socket_peer);
#else
    ::close(socket_peer);
#endif
    socket_peer = -1;
    printf("Finished.\n");
}

bool Socket::listen(int backlog)
{
    printf("Listening for incoming connections...\n");

    // Check if the socket has been created and bound before calling listen
    if (socket_peer == -1)
    {
        fprintf(stderr, "Socket is not created or not bound.\n");
        return false;
    }

    // Call the listen function on the socket
    if (::listen(socket_peer, backlog) == -1)
    {
        fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
        return false;
    }

    printf("Listening on the socket.\n");
    return true;
}

Socket Socket::accept()
{
    printf("Accepting incoming connection...\n");

    // Create a new Socket object to hold the client socket descriptor
    Socket clientSocket;

    // Accept the incoming connection and obtain the client socket descriptor
    struct sockaddr_in clientSockAddr;
    socklen_t clientSockAddrLen = sizeof(clientSockAddr);
    SOCKET clientSocketPeer = ::accept(socket_peer, (struct sockaddr *)&clientSockAddr, &clientSockAddrLen);

    if (!ISVALIDSOCKET(clientSocketPeer))
    {
        fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());
        return clientSocket; // Return an invalid client socket
    }

    clientSocket.socket_peer = clientSocketPeer;
    printf("Accepted connection from %s\n", inet_ntoa(clientSockAddr.sin_addr));

    return clientSocket;
}

int Socket::recv(void *buffer, int length)
{
    // Receive data from the socket
    int bytesReceived = ::recv(socket_peer, static_cast<char *>(buffer), length, 0);

    if (bytesReceived == -1)
    {
        fprintf(stderr, "recv() failed. (%d)\n", GETSOCKETERRNO());
    }

    return bytesReceived;
}

int Socket::send(const void *data, int length)
{
    // Send data to the connected socket
    int bytesSent = ::send(socket_peer, static_cast<const char *>(data), length, 0);

    if (bytesSent == -1)
    {
        fprintf(stderr, "send() failed. (%d)\n", GETSOCKETERRNO());
    }

    return bytesSent;
}

std::pair<std::string, std::string> Socket::configureLocalAddress(const std::string &port)
{
    // Implementation of configureLocalAddress function...
    printf("Configuring local address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    struct addrinfo *bind_address;
    if (getaddrinfo(0, port.c_str(), &hints, &bind_address))
    {
        fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
        return std::make_pair("", "");
    }

    char address_buffer[100];
    char service_buffer[100];
    getnameinfo(bind_address->ai_addr, bind_address->ai_addrlen, address_buffer, sizeof(address_buffer), service_buffer, sizeof(service_buffer), NI_NUMERICHOST | NI_NUMERICSERV);

    freeaddrinfo(bind_address);
    printf("Local address is: %s %s\n", address_buffer, service_buffer);
    return std::make_pair(address_buffer, service_buffer);
}

std::pair<std::string, std::string> Socket::configureRemoteAddress(const std::string &host, const std::string &port)
{
    // Implementation of configureRemoteAddress function...
    printf("Configuring remote address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo *peer_address;
    if (getaddrinfo(host.c_str(), port.c_str(), &hints, &peer_address))
    {
        fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
        return std::make_pair("", "");
    }

    char address_buffer[100];
    char service_buffer[100];
    getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen, address_buffer, sizeof(address_buffer), service_buffer, sizeof(service_buffer), NI_NUMERICHOST | NI_NUMERICSERV);

    printf("Remote address is: %s %s\n", address_buffer, service_buffer);
    return std::make_pair(address_buffer, service_buffer);
}
bool Socket::isValid() const
{

    return (socket_peer != -1);
}
