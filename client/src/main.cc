#include "client.h"

int main(int argc, char* argv[]) {
    cli_log->setOutstream("client.log");
    const char* server_host = "127.0.0.1"; // Replace with the server's IP address or hostname
    int server_port = 8080; // Replace with the server's port number

    Client client(server_host, server_port);
    client.run();

    return 0;
}
