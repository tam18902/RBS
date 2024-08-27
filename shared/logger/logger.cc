#include "logger.h"

std::unique_ptr<Logger> Logger::instance = nullptr;

/*
    Logger Implement
*/

std::string Logger::levelToString(Level level) const {
    switch (level) {
        case DEBUG:   return "DEBUG";
        case INFO:    return "INFO";
        case WARNING: return "WARNING";
        case ERROR:   return "ERROR";
        default:      return "UNKNOWN";
    }
}

std::string Logger::currentTime() const {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&time);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

Logger::Logger(_constructor_tag):outstream(&std::cerr),logLevel(Level::INFO){

};

Logger::~Logger() {
    if (instance->fileStream->is_open()) {
        instance->fileStream->close();
    }
}

Logger* Logger::getInstance() {
    if (instance == nullptr) {
        instance = std::make_unique<Logger>(_constructor_tag{});
    }
    return instance.get();
}

void Logger::setOutstream(const std::string& path) {
    if (instance == nullptr) {
        getInstance();
    }

    if (instance->fileStream != nullptr)
    {
        instance->fileStream->close();
    }

    instance->fileStream = std::make_unique<std::ofstream>(path, std::ios::out);
    // instance->fileStream = std::make_unique<std::ofstream>(path, std::ios::out | std::ios::app);
    
    if (instance->fileStream->is_open()) {
        instance->outstream = instance->fileStream.get();
    } else {
        std::cerr << "Failed to open log file: " << path << ". Falling back to std::cerr." << std::endl;
        instance->outstream = &std::cerr;
    }
}

void Logger::setLogLevel(Level level) {
    if (instance == nullptr) {
        getInstance();
    }
    instance->logLevel = level;
}

void Logger::logMessage(Level level, const std::string& message, const std::source_location& location) {
    if (level < logLevel)
        return;
    
    std::ostringstream oss;
    oss << "[" << currentTime() << "]"
        << "[" << levelToString(level) << "]"
        << "[" << location.file_name() << ":" << location.line() << "]"
        // << "[" << location.function_name() << "]"
        << " - " << message;
    *outstream << oss.str() << std::endl;
}

std::string Logger::formater(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    return std::string(buffer);
}

void Logger::debug(const std::string& message, std::source_location location) {
    logMessage(Level::DEBUG, message, location);
}

void Logger::info(const std::string& message, std::source_location location) {
    logMessage(Level::INFO, message, location);
}

void Logger::warning(const std::string& message, std::source_location location) {
    logMessage(Level::WARNING, message, location);
}

void Logger::error(const std::string& message, std::source_location location) {
    logMessage(Level::ERROR, message, location);
}


/*
    LogStream Implement
*/

LogStream::~LogStream() {
    if(!buffer.empty()) {
        logger.logMessage(bufferLevel, buffer, location);
        buffer.clear();
    }
}

LogStream& LogStream::operator<<(Level level) {
    bufferLevel = level;
    return *this;
}
LogStream& LogStream::operator<<(const std::string& value) {
    buffer += value;
    return *this;
}

LogStream& LogStream::operator<<(const char* value) {
    buffer += value;
    return *this;
}

LogStream& LogStream::operator<<(std::ostream& (*manip)(std::ostream&)) {
    if (manip == static_cast<std::ostream& (*)(std::ostream&)>(std::endl)) {
        logger.logMessage(bufferLevel, buffer, location);
        buffer.clear();
    }
    return *this;
}