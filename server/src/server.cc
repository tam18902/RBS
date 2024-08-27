#include "server.h"

Server::Server(int port) {
    clog->info(Logger::formater("Initialize server with port: %d", port));
    if (!server_socket.create() || !server_socket.bind(port) || !server_socket.listen(5)) {
        clog->error("Failed to initialize server.");
        std::cerr << "Failed to initialize server." << std::endl;
        exit(EXIT_FAILURE);
    }
}

Server::~Server() {
    clog->info("Destroy Server");
    clients.clear();
    server_socket.close();
}

void Server::run() {
    clog->info("Server is running and listening ...");
    std::cout << "Server is running and listening ..." << std::endl;
    std::thread(&Server::handleServer, this).detach();
    while (true) {
        auto client_socket = std::make_shared<Socket>();
        if (server_socket.accept(*client_socket)) {
            LOG << Level::INFO << "Accept client with socket.";
            clients.push_back(client_socket);
            LOG << Level::INFO << "Num of clients: " << clients.size();
            std::thread(&Server::handleClient, this, client_socket).detach();
        }
    }
}

void Server::handleServer() {
    bool exit_flag{false};
    std::string cmd;
    while (!exit_flag)
    {
        std::cout << "> ";
        std::getline(std::cin, cmd);
        LOG << Level::INFO << "Server Input: " << cmd << std::endl;
        if (cmd == "quit" || cmd == "q")
        {
            LOG << Level::INFO << "Server Exit Command" << std::endl;
            exit_flag = true;
        }
    }
    LOG << Level::INFO << "Server Bye." << std::endl;
    exit(EXIT_SUCCESS);
}

void Server::handleClient(std::shared_ptr<Socket> client_socket) {
    char buffer[1024];
    while (true) {
        std::string message;
        int bytes_received = client_socket->receive(message);
        LOG << Level::INFO << "Client Message" << Logger::formater("[%d Bytes]: ", bytes_received) << message << std::endl;
        if (bytes_received > 0) {
            std::cout << "Received: " << message << std::endl;
            if (message == "quit" || message == "q") {
                clog->info("Client exit command");
                break;
            }
            // // Broadcast message to all clients
            // for (const auto& client : clients) {
            //     if (client != client_socket) { // Avoid sending message back to the sender
            //         client->send(message);
            //     }
            // }
        } else {
            break; // Exit loop if no data is received or connection is closed
        }
    }

    // Remove client and close connection
    clog->info("Close connection.");
    clients.erase(
        std::remove_if(
            clients.begin(), 
            clients.end(),
            [&client_socket](const std::shared_ptr<Socket>& client) {
                return client == client_socket;
            }
        ),
        clients.end()
    );
    client_socket->close();
}