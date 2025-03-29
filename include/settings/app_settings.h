#pragma once
#include <boost/json.hpp>

struct AppSettings {
    const bool showFps;

    static AppSettings fromJson(const boost::json::object& json);
};
