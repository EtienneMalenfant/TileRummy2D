#pragma once
#include <string>

struct Action {
    int srcTileUID {-1};
    int destLeftTileUID {-1};
    int destRightTileUID {-1};

    std::string toString();

    ~Action() = default;
};
