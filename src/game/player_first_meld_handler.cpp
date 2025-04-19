#include <game/player_first_meld_handler.h>

// ------------------------------

PlayerFirstMeldHandler::PlayerFirstMeldHandler(IPlayerController* controller, IPlayer* player)
    : _controller(controller), _player(player) {}

PlayerFirstMeldHandler::~PlayerFirstMeldHandler() {
    delete _controller;
}

// ------------------------------

const Tile* PlayerFirstMeldHandler::getTile(int tileId) {
    for (const Tile* tile : *_player->getTiles()) {
        if (tile->uid == tileId) {
            return tile;
        }
    }
    for (const Tile* tile : _playedTiles) {
        if (tile->uid == tileId) {
            return tile;
        }
    }
    return nullptr;
}

bool PlayerFirstMeldHandler::isPlayingOnHisTiles(Action* action) {
    if (action->destLeftTileUID == -1 && action->destRightTileUID == -1) {
        return true;
    }
    for (const Tile* tile : _playedTiles) {
        if (tile->uid == action->destLeftTileUID || tile->uid == action->destRightTileUID) {
            return true;
        }
    }
    return false;
}

// ------------------------------

void PlayerFirstMeldHandler::draw() {
    _controller->draw();
}

bool PlayerFirstMeldHandler::addAction(Action* action) {
    if (_hasStarted == false) {
        if (isPlayingOnHisTiles(action) == false ) {
            return false;
        }
        const Tile* tile = getTile(action->srcTileUID);
        // Vérifier que la tuile provien de son jeu
        if (tile == nullptr) {
            return false;
        }
        _points += tile->value;
        _playedTiles.push_back(tile);
    }
    return _controller->addAction(action);
}

bool PlayerFirstMeldHandler::commitActions() {
    if (!_hasStarted) {
        if (_points >= POINTS_REQUIRED) {
            bool commitValid = _controller->commitActions();
            if (commitValid) {
                _hasStarted = true;
                _points = 0;
            }
            return commitValid;
        }
        return false;
    }
    return _controller->commitActions();
}

void PlayerFirstMeldHandler::cancelActions() {
    _points = 0;
    _controller->cancelActions();
}

bool PlayerFirstMeldHandler::isStarted() const {
    return _hasStarted;
}
