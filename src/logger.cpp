#include <logger.h>

using namespace std;

string ILogger::getTimeStamp() {
    time_t now = time(0);
    tm* localTime = localtime(&now);
    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localTime);
    return '[' + string(buffer) + "] ";
}

// ------------------------------

void ConsoleLogger::log(const string& message) {
    cout << getTimeStamp() <<  message << endl;
}

void ConsoleLogger::log(const string& message, LogType type) {
    log(message);
}

// ------------------------------

const string FileLogger::_logDir { "logs/" };

FileLogger::FileLogger(const string& filename, bool overwrite) {
    if (overwrite) {
        _file.open(_logDir + filename, ios::out | ios::trunc);
    } else {
        _file.open(_logDir + filename, ios::out | ios::app);
    }
}

FileLogger::~FileLogger() {
    _file.close();
}

void FileLogger::log(const string& message) {
    _file << getTimeStamp() << message << endl;
}

void FileLogger::log(const string& message, LogType type) {
    log(message);
}