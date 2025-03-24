#pragma once
#include <string>
#include <iostream>
#include <io/i_output_stream.h>

enum class LogType {
    Info,
    Important,
    Warning,
    Error
};

class ILogger {
protected:
    std::string getTimeStamp();
public:
    virtual void log(const std::string& message) = 0;
    virtual void log(const std::string& message, LogType type) = 0;
    virtual ~ILogger() = default;
};

class ConsoleLogger : public ILogger {
public:
    ConsoleLogger() = default;
    void log(const std::string& message) override;
    void log(const std::string& message, LogType type) override;
};

class FileLogger : public ILogger {
private:
    IOutputStream* _file;
    static const std::string _logDir;
public:
    FileLogger(const std::string& filename, bool overwrite = false);
    ~FileLogger() override;
    void log(const std::string& message) override;
    void log(const std::string& message, LogType type) override;
};
