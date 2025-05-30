#include <logger.h>
#include <io/file_output.h>
#include <io/paths.h>
#include <filesystem>

using namespace std;

string ILogger::getTimeStamp() {
    time_t now = time(0);
    tm* localTime = localtime(&now);
    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localTime);
    return '[' + string(buffer) + "] ";
}

string ILogger::getLogTypeString(LogType type) {
    switch (type) {
        case LogType::Info: return "[INFO] ";
        case LogType::Important: return "[IMPORTANT] ";
        case LogType::Warning: return "[WARNING] ";
        case LogType::Error: return "[ERROR] ";
        default: return "[UNKNOWN] ";
    }
}

// ------------------------------

void ConsoleLogger::log(const string& message) {
    cout << getTimeStamp() <<  message << endl;
}

void ConsoleLogger::log(const string& message, LogType type) {
    log(message);
}

// ------------------------------


FileLogger::FileLogger(const string& filename, bool overwrite) {
    if (!filesystem::exists(paths::logDir)) {
        filesystem::create_directories(paths::logDir);
    }
    _file = new FileOutput(paths::logDir + filename, overwrite);
}

FileLogger::~FileLogger() {
    delete _file;
}

void FileLogger::log(const string& message) {
    _file->writeLine(getTimeStamp() + message);
}

void FileLogger::log(const string& message, LogType type) {
    log(getLogTypeString(type) + message);
}
