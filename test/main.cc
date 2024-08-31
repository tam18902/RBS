#include "logger.h"
#include "database.h"

void testLogger();
void testDatabase();
int main() {
    testDatabase();
    return 0;
}

void testNestFunction(int a, float b) {
    LOG << Level::INFO << "Info message: " << a << ", " << b << std::endl;
    LOG << Level::WARNING << "This message is logged to a file." << std::endl;
}

void testLogger() {
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

}

void testDatabase() {
    // Create a new instance of the Database
    Database* db = Database::getInstance("database.txt");

    // Test size and isEmpty
    std::cout << "Testing size and isEmpty:" << std::endl;
    std::cout << "Size: " << db->size() << std::endl;
    std::cout << "Is Empty: " << (db->isEmpty() ? "Yes" : "No") << std::endl;
    
    // Test the get function
    std::cout << "Testing get:" << std::endl;
    std::string value = db->get("name");
    std::cout << "Value for 'name': " << value << std::endl;
    
    std::cout << "Testing printOut:" << std::endl;
    std::cout << db->toString();
}