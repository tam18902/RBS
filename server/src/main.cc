#include "server.h"

int main(int argc, char* argv[]) {
    server_log->setOutstream("server.log");
    Server server(8080);
    server.run();
    return 0;
}
