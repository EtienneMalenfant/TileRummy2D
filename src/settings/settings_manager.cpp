#include <settings/settings_loader.h>
#include <io/file_input.h>
#include <io/file_output.h>
#include <io/string_ops.h>

using namespace boost;

AppSettingsLoader::AppSettingsLoader(const std::string& fileName) : _fileName(fileName) {}

AppSettings AppSettingsLoader::loadSettings() {
    std::ifstream file(_fileName);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open settings file: " + _fileName);
    }
    std::string jsonContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    json::value jsonValue = json::parse(jsonContent);
    return AppSettings::fromJson(jsonValue.as_object());
}
