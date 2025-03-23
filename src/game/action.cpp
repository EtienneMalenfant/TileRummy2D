#include <game/action.h>

std::string Action::toString() {
    return "Action: srcTileUID: " + std::to_string(srcTileUID) + " destLeftTileUID: " + std::to_string(destLeftTileUID) + " destRightTileUID: " + std::to_string(destRightTileUID);    
}