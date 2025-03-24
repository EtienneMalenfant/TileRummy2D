#pragma once
#include <string>

class IInputStream {
public:
    virtual ~IInputStream() = default;

    virtual std::string readNext() = 0;
    virtual bool hasNext() = 0;
};
