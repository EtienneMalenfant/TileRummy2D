#include <game/player.h>
#include <stdexcept>

PlayerManager::PlayerManager(ITileStock* stock, IMeldsManager* meldsManager,
    IGameEventPublisher* eventPublisher, const std::string& name)
    : _stock(stock), _meldsManager(meldsManager), _eventPublisher(eventPublisher), _name(name) {
    for (int i = 0; i < 14; i++) {
        Tile* tile = _stock->draw();
        _tiles.push_back(tile);
    }
}

PlayerManager::~PlayerManager() {
    for (Tile* tile : _tiles) {
        delete tile;
    }
    for (Tile* tile : _standbyTiles) {
        delete tile;
    }
}

// ------------------------------

Tile* PlayerManager::getTile(int tileId) {
    for (Tile* tile : _tiles) {
        if (tile->uid == tileId) {
            return tile;
        }
    }
    return nullptr;
}

// ------------------------------

void PlayerManager::draw() {
    _meldsManager->cancel();
    _tiles.insert(_tiles.end(), _standbyTiles.begin(), _standbyTiles.end());
    _standbyTiles.clear();

    Tile* tile = _stock->draw();
    if (tile == nullptr) {
        _eventPublisher->notifyListeners(std::make_shared<CustomEvent>("Il n'y a plus de tuiles à piger"));
        return;
    }
    _tiles.push_back(tile);
    _eventPublisher->notifyListeners(std::make_shared<DrawEvent>(_name));
}


bool PlayerManager::addAction(Action* action) {
    Tile* tile = getTile(action->srcTileUID);
    if (tile != nullptr) {
        _tiles.remove(tile);
        _standbyTiles.push_back(tile);
        _meldsManager->putTile(action, tile);
    }
    else {
        _meldsManager->moveTile(action);
    }
    return true;
}

bool PlayerManager::commitActions() {
    // le jeu est valide et on a placé on moins une tuile
    if (_meldsManager->confirm() && _standbyTiles.size() > 0) {
        _standbyTiles.clear();
        _eventPublisher->notifyListeners(std::make_shared<PlayerPlayedEvent>(_name));
        return true;
    }
    return false;
}

void PlayerManager::cancelActions() {
    for (Tile* tile : _standbyTiles) {
        _tiles.push_back(tile);
    }
    _standbyTiles.clear();
    _meldsManager->cancel();
}

// IPlayer implementation

const std::list<const Tile*>* PlayerManager::getTiles() const {
    return reinterpret_cast<const std::list<const Tile*>*>(&_tiles);
}

std::string PlayerManager::getName() const {
    return _name;
}

bool PlayerManager::isStarted() const {
    return true;
}
