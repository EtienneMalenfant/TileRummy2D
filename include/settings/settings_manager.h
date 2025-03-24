#pragma once
#include <map>
#include <string>
#include <io/i_output_stream.h>

class ISettingsManager {
public:
    virtual ~ISettingsManager() = default;

    virtual std::string getSetting(const std::string& key) = 0;
    virtual void setSetting(const std::string& key, const std::string& value) = 0;
};

class SettingsManager : public ISettingsManager {
private:
    const std::string _fileName;
    std::map<std::string, std::string> _settings;
public:
    SettingsManager(const std::string& fileName);
    std::string getSetting(const std::string& key) override;
    void setSetting(const std::string& key, const std::string& value) override;
};
