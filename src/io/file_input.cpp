#include <io/file_input.h>
#include <settings/paths.h>

FileInput::FileInput(const std::string& path) {
    file = std::ifstream(settings::dataRoot + path);
}

FileInput::~FileInput() {
    file.close();
}

std::string FileInput::readNext() {
    std::string line;
    std::getline(file, line);
    return line;
}

bool FileInput::hasNext() {
    return !file.eof();
}
