#pragma once

#include <string>
#include <iostream>
#include <mutex>

namespace ve::core {

enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error
};

class Logger {
public:
    static void log(LogLevel level, const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex_);
        switch (level) {
            case LogLevel::Debug: std::cout << "[DEBUG] "; break;
            case LogLevel::Info: std::cout << "[INFO] "; break;
            case LogLevel::Warning: std::cerr << "[WARN] "; break;
            case LogLevel::Error: std::cerr << "[ERROR] "; break;
        }
        std::cout << message << std::endl;
    }

    static void info(const std::string& message) { log(LogLevel::Info, message); }
    static void error(const std::string& message) { log(LogLevel::Error, message); }

private:
    static std::mutex mutex_;
};

} // namespace ve::core
