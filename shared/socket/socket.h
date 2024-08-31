#ifndef _RBS_SOCKET
#define _RBS_SOCKET 1
#include <cstring>              //memset, memcpy
#include <unistd.h>             //close() function
#include <sys/socket.h>
#include <netdb.h>
#include <string>

static constexpr int BUFFER_SIZE = 1024;

class Socket {
protected:
    int sockfd;
    struct sockaddr_in addr;

public:
    Socket() : sockfd(-1) {}

    virtual ~Socket(){
        close();
    }

    bool create();

    bool bind(int port);

    bool listen(int backlog = 5) const;

    bool connect(const char* host, int port);

    bool accept(Socket &new_socket) const;

    // bool send(const char* message, size_t length) const;

    // int receive(char* buffer, size_t length) const;

    bool send(const std::string& message) const;

    int receive(std::string& message) const;
    
    void close();
};

#endif /* _RBS_SOCKET */
