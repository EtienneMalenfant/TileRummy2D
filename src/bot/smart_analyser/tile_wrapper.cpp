#include <bot/smart_analyser/tile_wrapper.h>
#include <stdexcept>

using namespace bot;

TileWrapper::TileWrapper(const Tile* tile, const IMeld* meld, bool isPlayerTile)
    : _currentTile(tile), meld(meld), isPlayerTile(isPlayerTile)
{}

TileWrapper::~TileWrapper() {
    delete _createdTile;
}

void TileWrapper::setJokerValue(int value, Color color) {
    if (_currentTile->isJoker == false) {
        throw new std::invalid_argument("Erreur: la tuile n'est pas un joker");
    }
    int id = _currentTile->uid;
    delete _createdTile;
    _currentTile = new Tile(value, color, true, id);
    _createdTile = _currentTile;
}
