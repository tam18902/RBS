#ifndef _RBS_LOGGER
#define _RBS_LOGGER 1

#include <cstdarg>
#include <chrono>
#include <fstream>
#include <memory>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <source_location>

enum Level {
    INFO,
    DEBUG, 
    WARNING, 
    ERROR
};

class Logger {
private:
    //For make_unique still class-scope constructor
    struct _constructor_tag { explicit _constructor_tag() = default; };
public:
    // Singleton template for Logger
    Logger(_constructor_tag);
    static Logger* getInstance();
    
    // Helper function with dynamic num of parameter
    static std::string formater(const char * format, ...);
    
    // Config Outstream and LogLevel
    void setOutstream(const std::string& path);
    void setLogLevel(Level level);
    
    // Destructor
    virtual ~Logger();

    // Public interface for end-user
    void debug(const std::string& message, std::source_location location = std::source_location::current());
    void info(const std::string& message, std::source_location location = std::source_location::current());
    void warning(const std::string& message, std::source_location location = std::source_location::current());
    void error(const std::string& message, std::source_location location = std::source_location::current());
    
    // Friend class for use operator << like a stream
    friend class LogStream;

private:
    static std::unique_ptr<Logger> instance;
    std::ostream* outstream;
    std::unique_ptr<std::ofstream> fileStream;
    Level logLevel;
    
    std::string currentTime() const;
    std::string levelToString(Level level) const;
    void logMessage(Level level, const std::string& message, const std::source_location& location);
};

class LogStream {
public:
    LogStream(Logger& logger, std::source_location location = std::source_location::current())
        : logger(logger), location(location), bufferLevel(Level::INFO) {}
    virtual ~LogStream();

    LogStream& operator<<(Level level);

    LogStream& operator<<(const std::string& value);

    LogStream& operator<<(const char* value);
    
    template<typename T>
    LogStream& operator<<(const T& value) {
        buffer += std::to_string(value);            
        return *this;
    }


    LogStream& operator<<(std::ostream& (*manip)(std::ostream&));

private:
    Logger& logger;
    std::source_location location;
    std::string buffer;
    Level bufferLevel;
};

#define LOG LogStream(*Logger::getInstance())

#endif /* _RBS_LOGGER */
