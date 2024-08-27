#include"socket.h"

bool Socket::create() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    return sockfd != -1;
}

bool Socket::bind(int port) {
    if (sockfd == -1) return false;

    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    return ::bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) != -1;
}

bool Socket::listen(int backlog) const {
    if (sockfd == -1) return false;
    return ::listen(sockfd, backlog) != -1;
}

bool Socket::connect(const char* host, int port) {
    if (sockfd == -1) return false;

    struct hostent *server = gethostbyname(host);
    if (server == nullptr) return false;

    std::memcpy(&addr.sin_addr.s_addr, server->h_addr, server->h_length);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    return ::connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) != -1;
}

bool Socket::accept(Socket &new_socket) const {
    socklen_t addr_len = sizeof(new_socket.addr);
    new_socket.sockfd = ::accept(sockfd, (struct sockaddr *)&new_socket.addr, &addr_len);
    return new_socket.sockfd != -1;
}

// bool Socket::send(const char* message, size_t length) const {
//     return ::write(sockfd, message, length) != -1;
// }

// int Socket::receive(char* buffer, size_t length) const {
//     std::memset(buffer, 0, length);
//     return ::read(sockfd, buffer, length - 1);
// }

bool Socket::send(const std::string& message) const {
    return ::write(sockfd, message.c_str(), message.length()) != -1;
}

int Socket::receive(std::string& message) const {
    message.clear();
    ssize_t total_byte{0};
    char buffer[BUFFER_SIZE + 1];
    bool exit_flag{false};
    do {
        std::memset(buffer, 0, BUFFER_SIZE);
        ssize_t num_byte = ::read(sockfd, buffer, BUFFER_SIZE);
        if (num_byte > 0) {
            total_byte += num_byte;
            buffer[num_byte] = '\0';
            message += buffer;   
        }
        
        if (num_byte < BUFFER_SIZE) {
            exit_flag = true;
        }
    } while(!exit_flag);

    return total_byte;
}

void Socket::close() {
    if (sockfd != -1) {
        ::close(sockfd);
        sockfd = -1;
    }
}
