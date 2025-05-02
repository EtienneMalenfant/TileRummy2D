#include <settings/settings_loader.h>

template <typename T>
BaseSettingsLoader<T>::BaseSettingsLoader(const std::string& fileName) : _file(fileName) {
    if (!_file.is_open()) {
        throw std::runtime_error("Could not open settings file: " + fileName);
    }
}

template <typename T>
boost::json::value BaseSettingsLoader<T>::getJsonContent() {
    std::string jsonContent((std::istreambuf_iterator<char>(_file)), std::istreambuf_iterator<char>());
    return boost::json::parse(jsonContent);
}

// ------------------------------

AppSettingsLoader::AppSettingsLoader(const std::string& fileName) : BaseSettingsLoader<AppSettings>(fileName) {}

AppSettings AppSettingsLoader::loadSettings() {
    return AppSettings::fromJson(getJsonContent().as_object());
}

// ------------------------------

GameSettingsLoader::GameSettingsLoader(const std::string& fileName) : BaseSettingsLoader<GameSettings>(fileName) {}

GameSettings GameSettingsLoader::loadSettings() {
    return GameSettings::fromJson(getJsonContent().as_object());
}
