#include <bot/actions_analyser.h>

using namespace bot;

void BaseActionsAnalyser::resetPlayerTiles() {
    _standbyTiles.clear();
    for (int i{0}; i < 4; i++) {
        std::fill(_tiles[i], _tiles[i] + 13, nullptr);
    }
    _joker[0] = nullptr;
    _joker[1] = nullptr;
    _jokerCount = 0;
}

void BaseActionsAnalyser::fillStandbyTiles(const std::list<const Tile*>* playerTiles) {
    for (const Tile* tile : *playerTiles) {
        _standbyTiles.push_back(tile);
    }
}

void BaseActionsAnalyser::updateTilesArray() {
    auto it = _standbyTiles.begin();
    while (it != _standbyTiles.end()) {
        const Tile* tile = *it;
        if (tile->isJoker) {
            _joker[_jokerCount] = tile;
            _jokerCount++;
            it = _standbyTiles.erase(it);
            continue;
        }
        if (_tiles[(int)tile->color][tile->value - 1] == nullptr) {
            _tiles[(int)tile->color][tile->value - 1] = tile;
            it = _standbyTiles.erase(it);
            continue;
        }
        it++;
    }
}

void BaseActionsAnalyser::emptyJokers() {
    _joker[0] = nullptr;
    _joker[1] = nullptr;
    _jokerCount = 0;
}
