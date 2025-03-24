#pragma once
#include <io/i_output_stream.h>
#include <fstream>

class FileOutput : public IOutputStream {
private:
    std::ofstream _file;
    void createDirectories(const std::string& path);
public:
    FileOutput(const std::string& path, bool overwrite = true);
    ~FileOutput() override;

    void write(const std::string& data) override;
    void writeLine(const std::string& data) override;
    void close() override;
};
