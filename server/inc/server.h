#ifndef _SERVER
#define _SERVER 1

#include "server_utils.h"

class Server {
private:
    Socket server_socket;
    std::vector<std::shared_ptr<Socket>> clients; // Use shared_ptr to manage dynamic memory

public:
    Server(int port);

    virtual ~Server();

    void run();

    void handleServer();

    void handleClient(std::shared_ptr<Socket> client_socket);
};

#endif /* _SERVER */