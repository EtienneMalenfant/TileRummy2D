#include <settings/app_settings.h>

AppSettings AppSettings::fromJson(const boost::json::object& json) {
    return AppSettings {
        showFPS: json.if_contains("showFps") ? json.at("showFps").as_bool() : false,
    };
}
