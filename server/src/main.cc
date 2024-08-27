#include "server.h"

extern Logger* clog = Logger::getInstance();

int main(int argc, char* argv[]) {
    clog->setOutstream("server.log");
    Server server(8080);
    server.run();
    return 0;
}
