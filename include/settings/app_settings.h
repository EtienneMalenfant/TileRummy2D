#pragma once
#include <string>
#include <array>
#include <boost/json.hpp>

struct AppSettings {
    const bool showFPS;
    const std::string playerName;
    const std::array<std::string,3> botNames;
    const int botCount;

    static AppSettings fromJson(const boost::json::object& json);
};
