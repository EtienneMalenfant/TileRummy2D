#include <settings/game_settings.h>

GameSettings GameSettings::fromJson(const boost::json::object& json) {
    int botCount = json.if_contains("botCount") ? json.at("botCount").to_number<int>() : 3;
    botCount = std::min(3, std::max(1, botCount)); // botCount should be between 1 and 3

    std::array<std::string, 3> botNamesStrings;
    const auto& botNamesArray = json.if_contains("botNames") ? json.at("botNames").as_array() : boost::json::array{};
    std::array<std::string, 3> defaultBotNames = {"Alice", "Bob", "Charlie"};
    for (size_t i = 0; i < botCount; ++i) {
        if (i < botNamesArray.size()) {
            botNamesStrings[i] = botNamesArray[i].as_string().c_str();
        } else {
            botNamesStrings[i] = defaultBotNames[i];
        }
    }

    return GameSettings {
        playerName: json.if_contains("playerName") ? json.at("playerName").as_string().c_str() : "Player",
        botNames: botNamesStrings,
        botCount: botCount
    };
}
