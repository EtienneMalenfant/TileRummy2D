#include <settings/settings_manager.h>
#include <io/file_input.h>
#include <io/file_output.h>
#include <io/string_ops.h>

SettingsManager::SettingsManager(const std::string& fileName) : _fileName(fileName) {
    IInputStream* fileIn = new FileInput(fileName);
    while (fileIn->hasNext()) {
        std::string line = fileIn->readNext();

        size_t pos = line.find(':');
        std::string key = line.substr(0, pos);
        stringOps::trim(key);

        std::string value = line.substr(pos + 1);
        stringOps::trim(value);

        _settings[key] = value;
    }
    delete fileIn;
}

std::string SettingsManager::getSetting(const std::string& key) {
    return _settings[key];
}

void SettingsManager::setSetting(const std::string& key, const std::string& value) {
    _settings[key] = value;

    IOutputStream* fileOut = new FileOutput(_fileName, true);
    for (const auto& setting : _settings) {
        fileOut->writeLine(setting.first + ": " + setting.second);
    }
}

