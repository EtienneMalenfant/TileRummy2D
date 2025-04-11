#include <io/file_output.h>
#include <filesystem>

FileOutput::FileOutput(const std::string& path, bool overwrite) {
    if (overwrite) {
        _file.open(path, std::ios::out | std::ios::trunc);
    } else {
        _file.open(path, std::ios::out | std::ios::app);
    }
}

FileOutput::~FileOutput() {
    _file.close();
}

void FileOutput::write(const std::string& data) {
    _file << data;
}

void FileOutput::writeLine(const std::string& data) {
    _file << data << std::endl;
}

void FileOutput::close() {
    _file.close();
}

// ------------------------------

void FileOutput::createDirectories(const std::string& path) {
    size_t pos = 0;
    while ((pos = path.find('/', pos)) != std::string::npos) {
        std::string dir = path.substr(0, pos);
        if (!dir.empty()) {
            std::filesystem::create_directory(dir);
        }
        pos++;
    }
}
