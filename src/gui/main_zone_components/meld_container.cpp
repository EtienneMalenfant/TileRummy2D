#include <gui/main_zone_components/meld_container.h>

using namespace gui;

MeldContainer::MeldContainer(const IMeld* meld, ITileFactory* tileFactory, IPlayerController* playerController)
    : _meld(meld), _tileFactory(tileFactory) {
    _tiles.reserve(7);
    Container::setFillColor(sf::Color::Transparent);
    _playerController = playerController;
    updateContainerSize();
}

MeldContainer::~MeldContainer() {
}

IPlayerController* MeldContainer::_playerController = nullptr;

// ------------------------------
//ITileContainer

Tile2D* MeldContainer::getTileAt(const sf::Vector2f& position) {
    for (Tile2D* tile : _tiles) {
        if (tile->getGlobalBounds().contains(position)) {
            return tile;
        }
    }
    return nullptr;
}

bool MeldContainer::addTileAt(Tile2D* tile, const sf::Vector2f& position) {
    sf::Vector2f containerPosition = Container::getPosition();
    int index = (position.x - containerPosition.x) / TileInfo::tileWidth;
    if (position.x < containerPosition.x) {
        index = -1;
    }

    int srcTileId = tile->getTileData()->uid;
    int leftNeighbourId = -1;
    if (index - 1 >= 0 && index - 1 < _tiles.size()) {
        leftNeighbourId = _tiles[index - 1]->getTileData()->uid;
        if (leftNeighbourId == srcTileId) {
            leftNeighbourId = -1;
        }
    }
    int rightNeighbourId = -1;
    if (index + 1 >= 0 && index + 1 < _tiles.size()) {
        rightNeighbourId = _tiles[index + 1]->getTileData()->uid;
        if (rightNeighbourId == srcTileId) {
            rightNeighbourId = -1;
        }
    }

    return _playerController->addAction(new Action{srcTileId, leftNeighbourId, rightNeighbourId});
}

void MeldContainer::previewTileAt(sf::Transformable& placeholder, const sf::Vector2f& position) {
    sf::Vector2f containerPosition = Container::getPosition();
    int index = (position.x - containerPosition.x) / TileInfo::tileWidth;
    if (position.x < containerPosition.x) {
        index = -1;
    }
    float x = containerPosition.x + (index * (int)TileInfo::tileWidth);
    placeholder.setPosition(x, containerPosition.y);
}

// ------------------------------

void MeldContainer::updateContainerSize() {
    Container::setSize(sf::Vector2f(TileInfo::tileWidth * _meld->size(), TileInfo::tileHeight));
}

void MeldContainer::updateTilesPosition() {
    sf::Vector2f position = Container::getPosition();
    for (int i{}; i < _tiles.size(); i++) {
        (_tiles)[i]->setPosition(position.x + (i * TileInfo::tileWidth), position.y);
    }
}

// ------------------------------
// sf::Shape

void MeldContainer::setPosition(float x, float y) {
    Container::setPosition(x, y);
    updateTilesPosition();
}

void MeldContainer::setPosition(const sf::Vector2f& position) {
    setPosition(position.x, position.y);
}

void MeldContainer::setFillColor(const sf::Color& color) {
    for (Tile2D* tile : _tiles) {
        tile->setFillColor(color);
    }
}

void MeldContainer::setSize(const sf::Vector2f& tileSize) {
    // pour les tuiles à l'intérieur seulement
    for (Tile2D* tile : _tiles) {
        tile->setSize(tileSize);
    }
    updateContainerSize();
}

void MeldContainer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // Container::draw(target, states);
    for (Tile2D* tile : _tiles) {
        target.draw(*tile, states);
    }
}

// ------------------------------

void MeldContainer::placeTile(const Tile* tileData, int index) {

    // on regarde si la tuile est déjà créée
    if (tilesById[tileData->uid] == nullptr) {
        // si non, on la crée
        tilesById[tileData->uid] = _tileFactory->createTile(tileData);
    }
    // on l'ajoute à la liste
    _tiles.insert(_tiles.begin() + index, tilesById[tileData->uid]);
}

// ------------------------------

void MeldContainer::update() {
    // on passe sur chaque tuile du meld
    for (int i = 0; i < _meld->size(); i++) {
        const Tile* meldTile = _meld->get(i);
        // on a déjà une tuile 2d à cet endroit
        if (_tiles.size() > i) {

            // si la tuile affichée n'est pas la bonne
            Tile2D* tile2d = _tiles[i];
            if (meldTile->uid != tile2d->getTileData()->uid) {
                // on la remplace
                placeTile(meldTile, i);
            }
        }
        else { // la tuile 2d n'est pas placée
            placeTile(meldTile, i);
        }
    }

    // enlever les tuiles en trop si le meld a été réduit
    for (int i = _meld->size(); i < _tiles.size(); i) {
        _tiles.erase(_tiles.begin() + i);
    }

    if (_tiles.size() > 0) {
        updateTilesPosition(); // on replace les tuiles
        updateContainerSize(); // important pour le placement du meld dans la grille du jeu
    }
}
