#include <tests/tests_declaration.h>
#include <tests/test.h>
#include <settings/settings_loader.h>

namespace SettingsTest {

    const std::string mockSettingsDir = "src/tests/settings/";

    void getSettingsFromFullJsonFile() {
        AppSettingsLoader settingsLoader(mockSettingsDir + "full_settings.json");
        AppSettings settings = settingsLoader.loadSettings();
        Test::validate(settings.showFPS == true, __func__);
        Test::validate(settings.playerName == "TestUser", __func__);
        Test::validate(settings.botCount == 3, __func__);
        Test::validate(settings.botNames[0] == "Bot1", __func__);
        Test::validate(settings.botNames[1] == "Bot2", __func__);
        Test::validate(settings.botNames[2] == "Bot3", __func__);
    }

    void getSettingsFromPartialJsonFile() {
        AppSettingsLoader settingsLoader(mockSettingsDir + "partial_settings.json");
        AppSettings settings = settingsLoader.loadSettings();
        Test::validate(settings.showFPS == true, __func__);
        Test::validate(settings.playerName == "TestUser", __func__);
        Test::validate(settings.botCount == 3, __func__);
        Test::validate(settings.botNames[0] == "Alice", __func__);
        Test::validate(settings.botNames[1] == "Bob", __func__);
        Test::validate(settings.botNames[2] == "Charlie", __func__);
    }

    void getSettingsFromEmptyJsonFile() {
        AppSettingsLoader settingsLoader(mockSettingsDir + "empty_settings.json");
        AppSettings settings = settingsLoader.loadSettings();
        Test::validate(settings.showFPS == false, __func__);
        Test::validate(settings.playerName == "Player", __func__);
        Test::validate(settings.botCount == 3, __func__);
        Test::validate(settings.botNames[0] == "Alice", __func__);
        Test::validate(settings.botNames[1] == "Bob", __func__);
        Test::validate(settings.botNames[2] == "Charlie", __func__);
    }

    void getSettingsWithOneBot() {
        AppSettingsLoader settingsLoader(mockSettingsDir + "one_bot_settings.json");
        AppSettings settings = settingsLoader.loadSettings();;
        Test::validate(settings.botCount == 1, __func__);
        Test::validate(settings.botNames[0] == "Bot", __func__);
    }

    void runTests() {
        getSettingsFromFullJsonFile();
        getSettingsFromPartialJsonFile();
        getSettingsFromEmptyJsonFile();
        getSettingsWithOneBot();
    }
}
