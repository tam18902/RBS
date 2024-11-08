#ifndef _SERVER
#define _SERVER 1

#include "server_utils.h"

class Server {
private:
    Socket server_socket;
    std::atomic<bool> running{true};                   // Flag to indicate if the server is running
    std::thread server_handle_thread;                  // Server thread
    std::vector<std::thread> clients_threads;           // Vector of client threads
    std::vector<std::shared_ptr<Socket>> clients;      // List of clients
    std::condition_variable exit_condition;            // Condition variable for notifying threads to exit
    std::mutex mtx;                                    // Mutex for condition variable
    void stop();
public:
    Server(int port);

    virtual ~Server();

    void run();

    void handleServer();

    void handleClient(std::shared_ptr<Socket> client_socket);
};

#endif /* _SERVER */