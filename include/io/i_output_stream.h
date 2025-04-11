#pragma once
#include <string>

class IOutputStream {
public:
    virtual ~IOutputStream() = default;

    virtual void write(const std::string& data) = 0;
    virtual void writeLine(const std::string& data) = 0;
    virtual void close() = 0;
};
