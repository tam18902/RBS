#ifndef _CLIENT
#define _CLIENT 1

#include "client_utils.h"

class Client {
private:
    Socket client_socket;
public:
    Client(const char* host, int port);

    virtual ~Client();

    void run();

    void receiveMessages();
};

#endif /* _CLIENT */