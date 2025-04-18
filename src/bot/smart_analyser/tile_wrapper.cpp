#include <bot/smart_analyser/tile_wrapper.h>
#include <stdexcept>

using namespace bot;

TileWrapper::TileWrapper(const Tile* tile, const IMeld* meld, bool isPlayerTile)
    : _tile(tile), meld(meld), isPlayerTile(isPlayerTile)
{}

TileWrapper::~TileWrapper() {
    if (_isCreatedTile) {
        delete _tile;
    }
}

void TileWrapper::setJokerValue(int value, Color color) {
    if (_tile->isJoker == false) {
        throw new std::invalid_argument("Erreur: la tuile n'est pas un joker");
    }
    int id = _tile->uid;
    if (_isCreatedTile) {
        delete _tile;
    }
    _tile = new Tile(value, color, true, id);
    _isCreatedTile = true;
}
