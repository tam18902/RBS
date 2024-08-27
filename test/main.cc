#include "logger.h"

void testNestFunction(int a, float b);
int main() {
    Logger* clog = Logger::getInstance();

    int a = 8;
    float b = 10.6;
    
    // std::string formatedMsg = Logger::formater("Debug message: a = %d, b = %f", a, b);
    // clog->debug(formatedMsg);
    clog->debug(Logger::formater("Debug message: a = %d, b = %f", a, b));

    // When use LOG/LogStream make sure std::endl for flush buffer
    LOG << Level::DEBUG << "Info message: " << a << ", " << b << std::endl; 

    LOG << "TestForNoEndl";


    // Optional: Initialize a file stream if you want to log to a file
    clog->setOutstream("logfile.log");

    // Set min Level for Log
    clog->setLogLevel(Level::DEBUG);

    testNestFunction(a, b);

    return 0;
}

void testNestFunction(int a, float b) {
    LOG << Level::INFO << "Info message: " << a << ", " << b << std::endl;
    LOG << Level::WARNING << "This message is logged to a file." << std::endl;
}