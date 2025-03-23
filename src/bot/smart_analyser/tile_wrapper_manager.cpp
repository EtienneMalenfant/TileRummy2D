#include <bot/smart_analyser/tile_wrapper_manager.h>

using namespace bot;

TileWrapperManager::TileWrapperManager(IMeldsContainer* meldsContainer) : _meldsContainer(meldsContainer) {}

void TileWrapperManager::resertArrays() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 14; j++) {
            for (int k = 0; k < 2; k++) {
                delete _tiles[i][j][k];
                _tiles[i][j][k] = nullptr;
            }
        }
    }
    for (int i = 0; i < 2; i++) {
        delete _jokers[i];
        _jokers[i] = nullptr;
    }
    // les tuiles du joueurs sont delete avec les tableau de tous les tuiles
    _playerTiles.clear();
}

void TileWrapperManager::fillGameTiles() {
    const std::list<const IMeld*>* melds = _meldsContainer->getMelds();
    for (const IMeld* meld : *melds) {
        for (int i = 0; i < meld->size(); i++) {
            addWrapper(new TileWrapper(meld->get(i), meld, false));
        }
    }
}

void TileWrapperManager::addWrapper(TileWrapper* tileWrapper) {
    if (tileWrapper->tile->isJoker) {
        int i = 0;
        if (_jokers[i] != nullptr) {
            i++;
        }
        _jokers[i] = tileWrapper;
    }
    else {
        int i = 0;
        if (_tiles[(int)tileWrapper->tile->color][tileWrapper->tile->value - 1][i] != nullptr) {
            i++;
        }
        _tiles[(int)tileWrapper->tile->color][tileWrapper->tile->value - 1][i] = tileWrapper;
    }
}

std::array<TileWrapper*,2> TileWrapperManager::getWrappers(Color color, int value) {
    std::array<TileWrapper*,2> wrappers;
    for (int i = 0; i < 2; i++) {
        wrappers[i] = _tiles[(int)color][value - 1][i];
    }
    return wrappers;
}

std::vector<TileWrapper*>* TileWrapperManager::getSetCompatibleTiles(Color color, int value) {
    std::vector<TileWrapper*>* compatibleTiles = new std::vector<TileWrapper*>();
    for (int colorIndex = 0; colorIndex < 4; colorIndex++) {
        if (colorIndex == (int)color) {
            continue;
        }
        std::array<TileWrapper*,2> wrappers = getWrappers((Color)colorIndex, value);
        for (int i = 0; i < 2; i++) {
            if (wrappers[i] != nullptr) {
                compatibleTiles->push_back(wrappers[i]);
            }
        }
    }
    return compatibleTiles;
}

std::vector<TileWrapper*>* TileWrapperManager::getSequenceCompatibleTiles(Color color, int value) {
    std::vector<TileWrapper*>* compatibleTiles = new std::vector<TileWrapper*>();
    for (int compatibleValue = std::max(value - 1, 1); compatibleValue <= std::min(value + 1, 13); compatibleValue++) {
        if (compatibleValue == value) {
            continue;
        }
        std::array<TileWrapper*,2> wrappers = getWrappers(color, compatibleValue);
        for (int i = 0; i < 2; i++) {
            if (wrappers[i] != nullptr) {
                compatibleTiles->push_back(wrappers[i]);
            }
        }
    }
    return compatibleTiles;
}


// ------------------------------


void TileWrapperManager::refresh(const std::list<const Tile*>* playerTiles) {
    resertArrays();
    fillGameTiles();
    for (const Tile* playerTile : *playerTiles) {
        TileWrapper* wrapperTile = new TileWrapper(playerTile, nullptr, true);
        _playerTiles.push_back(wrapperTile);
        addWrapper(wrapperTile);
    }
}

const std::vector<TileWrapper*>* TileWrapperManager::getPlayerTiles() {
    return &_playerTiles;
}

const std::vector<TileWrapper*>* TileWrapperManager::getCompatibleTiles(const Tile* tile) {
    // renvoyer toutes les tuiles si c'est un joker
    if (tile->isJoker) {
        std::vector<TileWrapper*>* compatibleTiles = new std::vector<TileWrapper*>();
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 13; j++) {
                for (int k = 0; k < 2; k++) {
                    if (_tiles[i][j][k] != nullptr) {
                        compatibleTiles->push_back(_tiles[i][j][k]);
                    }
                }
            }
        }
        for (int i = 0; i < 2; i++) {
            if (_jokers[i] != nullptr && _jokers[i]->tile != tile) {
                compatibleTiles->push_back(_jokers[i]);
            }
        }
        return compatibleTiles;
    }
    std::vector<TileWrapper*>* compatibleTiles = getSetCompatibleTiles(tile->color, tile->value);
    std::vector<TileWrapper*>* sequenceCompatibleTiles = getSequenceCompatibleTiles(tile->color, tile->value);
    for (TileWrapper* tile : *sequenceCompatibleTiles) {
        compatibleTiles->push_back(tile);
    }
    delete sequenceCompatibleTiles;
    for (int i = 0; i < 2; i++) {
        if (_jokers[i] != nullptr) {
            compatibleTiles->push_back(_jokers[i]);
        }
    }
    return compatibleTiles;
};
