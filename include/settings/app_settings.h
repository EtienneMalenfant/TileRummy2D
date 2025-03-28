#pragma once
#include <boost/json.hpp>

struct AppSettings {
    const bool showFPS;

    static AppSettings fromJson(const boost::json::object& json);
};
