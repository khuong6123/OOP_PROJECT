#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger {
public:
    static Logger& GetInstance();
    
    void SetLogFile(const std::string& filename);
    void SetMinLevel(LogLevel level);
    void Log(LogLevel level, const std::string& message);
    
private:
    Logger() = default;
    ~Logger();
    
    std::ofstream logFile_;
    LogLevel minLevel_ = LogLevel::INFO;
    std::mutex mutex_;
    
    std::string LevelToString(LogLevel level);
};

#define LOG_DEBUG(msg) Logger::GetInstance().Log(LogLevel::DEBUG, msg)
#define LOG_INFO(msg) Logger::GetInstance().Log(LogLevel::INFO, msg)
#define LOG_WARNING(msg) Logger::GetInstance().Log(LogLevel::WARNING, msg)
#define LOG_ERROR(msg) Logger::GetInstance().Log(LogLevel::ERROR, msg)

#endif // LOGGER_H