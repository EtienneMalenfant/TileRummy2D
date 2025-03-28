#include <tests/tests_declaration.h>
#include <tests/test.h>
#include <settings/settings_loader.h>

namespace SettingsTest {

    const std::string mockSettingsDir = "src/tests/settings/";

    void getAppSettingsFromFullJsonFile() {
        AppSettingsLoader settingsLoader(mockSettingsDir + "app_settings/full_settings.json");
        AppSettings settings = settingsLoader.loadSettings();
        Test::validate(settings.showFPS == true, __func__);
    }

    void getAppSettingsFromEmptyJsonFile() {
        AppSettingsLoader settingsLoader(mockSettingsDir + "empty_settings.json");
        AppSettings settings = settingsLoader.loadSettings();
        Test::validate(settings.showFPS == false, __func__);
    }

    void runAppSettingsTests() {
        getAppSettingsFromFullJsonFile();
        getAppSettingsFromEmptyJsonFile();
    }
}
