#include <tests/tests_declaration.h>
#include <tests/test.h>
#include <settings/settings_loader.h>

namespace SettingsTest {
    const std::string mockSettingsDir = "src/tests/settings/";

    void getGameSettingsFromFullJsonFile() {
        GameSettingsLoader settingsLoader(mockSettingsDir + "game_settings/full_settings.json");
        GameSettings settings = settingsLoader.loadSettings();
        Test::validate(settings.playerName == "TestUser", __func__);
        Test::validate(settings.botCount == 3, __func__);
        Test::validate(settings.botNames[0] == "Bot1", __func__);
        Test::validate(settings.botNames[1] == "Bot2", __func__);
        Test::validate(settings.botNames[2] == "Bot3", __func__);
    }

    void getGameSettingsFromPartialJsonFile() {
        GameSettingsLoader settingsLoader(mockSettingsDir + "game_settings/partial_settings.json");
        GameSettings settings = settingsLoader.loadSettings();
        Test::validate(settings.playerName == "TestUser", __func__);
        Test::validate(settings.botCount == 3, __func__);
        Test::validate(settings.botNames[0] == "Alice", __func__);
        Test::validate(settings.botNames[1] == "Bob", __func__);
        Test::validate(settings.botNames[2] == "Charlie", __func__);
    }

    void getGameSettingsWithOneBot() {
        GameSettingsLoader settingsLoader(mockSettingsDir + "game_settings/one_bot_settings.json");
        GameSettings settings = settingsLoader.loadSettings();;
        Test::validate(settings.botCount == 1, __func__);
        Test::validate(settings.botNames[0] == "Bot", __func__);
    }

    void getGameSettingsFromEmptyJsonFile() {
        GameSettingsLoader settingsLoader(mockSettingsDir + "empty_settings.json");
        GameSettings settings = settingsLoader.loadSettings();
        Test::validate(settings.playerName == "Player", __func__);
        Test::validate(settings.botCount == 3, __func__);
        Test::validate(settings.botNames[0] == "Alice", __func__);
        Test::validate(settings.botNames[1] == "Bob", __func__);
        Test::validate(settings.botNames[2] == "Charlie", __func__);
    }

    void runGameSettingsTests() {
        getGameSettingsFromFullJsonFile();
        getGameSettingsFromPartialJsonFile();
        getGameSettingsFromEmptyJsonFile();
        getGameSettingsWithOneBot();
    }
}
