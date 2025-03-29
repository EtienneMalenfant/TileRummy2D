#pragma once
#include <string>
#include <fstream>
#include <settings/app_settings.h>
#include <settings/game_settings.h>

template <typename T>
class ISettingsLoader {
public:
    virtual T loadSettings() = 0;
    virtual ~ISettingsLoader() = default;
};

template <typename T>
class BaseSettingsLoader : public ISettingsLoader<T> {
protected:
    std::ifstream _file;
public:
    BaseSettingsLoader(const std::string& fileName);
    ~BaseSettingsLoader();
    boost::json::value getJsonContent();
};

class AppSettingsLoader : public BaseSettingsLoader<AppSettings> {
public:
    AppSettingsLoader(const std::string& fileName);
    AppSettings loadSettings() override;
};

class GameSettingsLoader : public BaseSettingsLoader<GameSettings> {
public:
    GameSettingsLoader(const std::string& fileName);
    GameSettings loadSettings() override;
};
