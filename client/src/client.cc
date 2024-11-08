#include "client.h"

Client::Client(const char* host, int port) {
    LOG << Level::INFO << "Init Client with host: " << host << ", port " << port << std::endl;
    client_socket.create();
    if (!client_socket.connect(host, port)) {
        cli_log->error("Failed to connect to server.");
        // std::cerr << "Failed to connect to server." << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

Client::~Client() {
    cli_log->info("Destroy Client.");
    client_socket.close();
}

void Client::run() {
    cli_log->info("Client Start.");
    // std::thread receiveThread = std::thread(&Client::receiveMessages, this);

    std::string message;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, message);
        cli_log->info(Logger::formater("Input message: %s", message.c_str()));
        if (message.empty()) {
            continue;
        }

        if (client_socket.send(message) == -1) {
            cli_log->error("Failed to send message.");
            // std::cerr << "Failed to send message." << std::endl;
            break;
        }
        int bytes_received = client_socket.receive(message);
        if (!message.empty()) {
            cli_log->info("Server" + message);
            std::cout << "Server: " << message << std::endl;
            std::cout << "> ";
        } else if (bytes_received == 0 && (errno == EWOULDBLOCK || errno == EAGAIN)) {
            
            LOG << Level::INFO << "Receive time out " << std::endl;
            continue;
        } else {
            cli_log->error("Connection closed or error occurred.");
            std::cerr << "Connection closed or error occurred." << std::endl;
            break;
        }
        if (message == "quit" || message == "q")
        {
            cli_log->info("Quit message.");
            std::cout << "Bye." << std::endl;
            break;
        }
    }
    client_socket.close();
    // if (receiveThread.joinable())
    // {
    //     receiveThread.join();
    // }
}

void Client::receiveMessages() {
    std::string message;
    while (true) {
        int bytes_received = client_socket.receive(message);
        if (!message.empty()) {
            cli_log->info("Server" + message);
            std::cout << "Server: " << message << std::endl;
            std::cout << "> ";
        } else if (bytes_received == 0 && (errno == EWOULDBLOCK || errno == EAGAIN)) {
            
            LOG << Level::INFO << "Receive time out " << std::endl;
            continue;
        } else {
            cli_log->error("Connection closed or error occurred.");
            std::cerr << "Connection closed or error occurred." << std::endl;
            break;
        }
    }
    client_socket.close();
}
