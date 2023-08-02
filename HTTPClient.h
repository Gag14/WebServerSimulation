#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include "Socket.h"
#include <string>

class HTTPClient {
private:
    Socket clientSocket;

public:
    HTTPClient();
    ~HTTPClient();

    bool connectToServer(const std::string& host, const std::string& port);
    void performGETRequest();
};

#endif // HTTPCLIENT_H

