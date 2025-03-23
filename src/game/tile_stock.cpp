#include <game/tile_stock.h>
#include <stdexcept>

TileStock::TileStock() {
    buildStock();
    _seed = std::random_device()();
    _randGen.seed(_seed);
}

TileStock::TileStock(ILogger* logger) : _logger(logger) {
    buildStock();
    std::random_device rd;
    _seed = rd();
    _randGen.seed(_seed);
    _logger->log("Seed générée du stock de tuiles : " + std::to_string(_seed));
}

TileStock::~TileStock() {
    for (int i = 0; i < 106; i++) {
        delete _tiles[i];
    }
}


void TileStock::buildStock() {
    Tile::nextTileUID = 0;
    unsigned int index {0};
    for (int nbPerTile {0}; nbPerTile < 2; nbPerTile++) {
        for (int nbColor {0}; nbColor < 4; nbColor++) {
            for (int value {1}; value <= 13; value++) {
                _tiles[index] = new Tile(value, (Color)nbColor, false);
                index++;
            }
        }
    }
    _tiles[index] = new Tile(0, Color::BLACK, true);
    index++;
    _tiles[index] = new Tile(0, Color::RED, true);
}

// ------------------------------

void TileStock::setSeed(unsigned int seed) {
    _seed = seed;
    _randGen.seed(_seed);
    _logger->log("Seed du stock de tuiles changée pour : " + std::to_string(_seed));
}

Tile* TileStock::draw() {
    if (_lastTileIndex < 0) {
        if (_logger != nullptr) {
            _logger->log(std::to_string(_seed) + ": Plus de tuiles à piger");
        }

        return nullptr;
    }
    // piger un nb aléatoire
    int index = 0;
    if (_lastTileIndex > 0) {
        std::uniform_int_distribution<> distribution(0, _lastTileIndex);
        index = distribution(_randGen);
    }

    Tile* tile = _tiles[index];
    // déplacer la dernière tuile à la place de celle pigée
    _tiles[index] = _tiles[_lastTileIndex];
    _tiles[_lastTileIndex] = nullptr;
    _lastTileIndex--;
    return tile;
}
