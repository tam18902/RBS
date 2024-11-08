#include "server.h"
#include <sys/select.h>

Server::Server(int port) {
    server_log->info(Logger::formater("Initialize server with port: %d", port));
    if (!server_socket.create() || !server_socket.bind(port) || !server_socket.listen(5)) {
        server_log->error("Failed to initialize server.");
        std::cerr << "Failed to initialize server." << std::endl;
        exit(EXIT_FAILURE);
    }
}

Server::~Server() {
    server_log->info("Destroy Server");
    stop();
    server_socket.close();
}

void Server::run() {
    struct timeval timeout;
    timeout.tv_sec = 5;  // Set timeout duration in seconds
    timeout.tv_usec = 0; // Timeout in microseconds
    server_log->info("Server is running and listening ...");
    std::cout << "Server is running and listening ..." << std::endl;
    running = true;
    server_handle_thread = std::thread(&Server::handleServer, this);
    
    while (running) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(server_socket.getFd(), &readfds);

        // Use select to wait for the socket to be ready for accepting a connection
        int activity = select(server_socket.getFd() + 1, &readfds, nullptr, nullptr, &timeout);

        if (activity < 0) {
            std::cerr << "Select error" << std::endl;
            break;
        }
        
        // If select returns 0, it means the timeout occurred
        if (activity == 0) {
            // std::cout << "Timeout occurred, no incoming connections." << std::endl;
            continue;
        }
        // If the server socket is ready for accepting a connection
        if (FD_ISSET(server_socket.getFd(), &readfds)) {
            auto client_socket = std::make_shared<Socket>();
            if (server_socket.accept(*client_socket)) {
                LOG << Level::INFO << "Accepted client with socket.";
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    clients.push_back(client_socket);
                    clients_threads.emplace_back(&Server::handleClient, this, client_socket);
                    LOG << Level::INFO << "Number of clients: " << clients.size();
                }
            }
        }
    }
    LOG << Level::INFO << "Server not waiting anymore" << std::endl;
}

void Server::stop() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        running = false;
    }
    exit_condition.notify_all();

    // Join the server handle thread to cleanly shut down
    if (server_handle_thread.joinable()) {
        server_handle_thread.join();
    }

    // Clean up client threads
    for (auto& thread : clients_threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    clients.clear();
    clients_threads.clear();
    LOG << Level::INFO << "Server stopped, all client threads exited.";
}

void Server::handleServer() {
    std::string cmd;
    while (running) {
        std::cout << "> ";
        std::getline(std::cin, cmd);
        LOG << Level::INFO << "Server Input: " << cmd << std::endl;

        if (cmd == "quit" || cmd == "q") {
            LOG << Level::INFO << "Server Exit Command" << std::endl;
            break;  // Exit the command loop
        }
    }
    // Notify all client threads to exit
    {
        std::lock_guard<std::mutex> lock(mtx);
        running = false;
    }
    exit_condition.notify_all();
    LOG << Level::INFO << "Server Bye." << std::endl;
}

void Server::handleClient(std::shared_ptr<Socket> client_socket) {
    while (running) {
        std::string message;
        int bytes_received = client_socket->receive(message);
        LOG << Level::INFO << "Client Message" << Logger::formater("[%d Bytes]: ", bytes_received) << message << std::endl;
        if (bytes_received > 0) {
            std::cout << "Received: " << message << std::endl;
            if (message == "quit" || message == "q") {
                server_log->info("Client exit command");
                break;
            }
            client_socket->send("Reply for " + message);
            // // Broadcast message to all clients
            // for (const auto& client : clients) {
            //     if (client != client_socket) { // Avoid sending message back to the sender
            //         client->send(message);
            //     }
            // }
        } else if (bytes_received == 0 && (errno == EWOULDBLOCK || errno == EAGAIN)) {
            std::cout << "Receive time out " << std::endl;
            continue;
        } else {
            break;
        }
    }

    // Remove client and close connection
    {
        std::lock_guard<std::mutex> lock(mtx);
        server_log->info("Close connection.");
        clients.erase(std::remove(clients.begin(), clients.end(), client_socket), clients.end());
    }
    client_socket->close();
}