#include "HTTPClient.h"

int main() {
    HTTPClient client;
    if (!client.connectToServer("127.0.0.1", "8080")) {
        return 1;
    }

    client.performGETRequest();

    return 0;
}

