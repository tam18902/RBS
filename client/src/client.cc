#include "client.h"

Client::Client(const char* host, int port) {
    LOG << Level::INFO << "Init Client with host: " << host << ", port " << port << std::endl;
    client_socket.create();
    if (!client_socket.connect(host, port)) {
        clog->error("Failed to connect to server.");
        // std::cerr << "Failed to connect to server." << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

Client::~Client() {
    clog->info("Destroy Client.");
    client_socket.close();
}

void Client::run() {
    clog->info("Client Start.");
    std::thread(&Client::receiveMessages, this).detach();

    std::string message;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, message);
        clog->info(Logger::formater("Input message: %s", message.c_str()));
        if (message.empty()) {
            continue;
        }

        if (client_socket.send(message) == -1) {
            clog->error("Failed to send message.");
            // std::cerr << "Failed to send message." << std::endl;
            break;
        }

        if (message == "quit" || message == "q")
        {
            clog->info("Quit message.");
            std::cout << "Bye." << std::endl;
            break;
        }
    }
    client_socket.close();
}

void Client::receiveMessages() {
    std::string message;
    while (true) {
        int bytes_received = client_socket.receive(message);
        if (!message.empty()) {
            clog->info("Server" + message);
            std::cout << "Server: " << message << std::endl;
            std::cout << "> ";
        } else {
            clog->error("Connection closed or error occurred.");
            std::cerr << "Connection closed or error occurred." << std::endl;
            break;
        }
    }
    client_socket.close();
}
