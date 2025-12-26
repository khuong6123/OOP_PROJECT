#include "Logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>

Logger& Logger::GetInstance() {
    static Logger instance;
    return instance;
}

Logger::~Logger() {
    if (logFile_.is_open()) {
        logFile_.close();
    }
}

void Logger::SetLogFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (logFile_.is_open()) {
        logFile_.close();
    }
    logFile_.open(filename, std::ios::app);
}

void Logger::SetMinLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(mutex_);
    minLevel_ = level;
}

void Logger::Log(LogLevel level, const std::string& message) {
    if (level < minLevel_) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&time);
    
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    ) % 1000;
    
    std::ostringstream logEntry;
    logEntry << "[" << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") 
             << "." << std::setfill('0') << std::setw(3) << ms.count() << "] "
             << "[" << LevelToString(level) << "] "
             << message << std::endl;
    
    // Output to console
    std::cout << logEntry.str();
    
    // Output to file if open
    if (logFile_.is_open()) {
        logFile_ << logEntry.str();
        logFile_.flush();
    }
}

std::string Logger::LevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG:    return "DEBUG";
        case LogLevel::INFO:     return "INFO";
        case LogLevel::WARNING:  return "WARNING";
        case LogLevel::ERROR:    return "ERROR";
        default:                 return "UNKNOWN";
    }
}