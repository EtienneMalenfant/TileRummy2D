#pragma once
#include <string>
#include <array>
#include <boost/json.hpp>

struct GameSettings {
    const std::string playerName;
    const std::array<std::string,3> botNames;
    const int botCount;

    static GameSettings fromJson(const boost::json::object& json);
};
