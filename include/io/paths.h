#pragma once

#include <string>

namespace paths {
    const std::string getExecutablePath();

    static const std::string executablePath = getExecutablePath();
#ifdef RELEASE
    static const std::string dataRoot = executablePath + "/data/";
#else
    static const std::string dataRoot = executablePath + "/../data/";
#endif
}
