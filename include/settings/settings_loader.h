#pragma once
#include <string>
#include <io/i_output_stream.h>
#include <settings/app_settings.h>

class IAppSettingLoader {
public:
    virtual ~IAppSettingLoader() = default;

    virtual AppSettings loadSettings() = 0;
};

class AppSettingsLoader : public IAppSettingLoader {
private:
    const std::string _fileName;


public:
    AppSettingsLoader(const std::string& fileName);
    AppSettings loadSettings() override;
};
