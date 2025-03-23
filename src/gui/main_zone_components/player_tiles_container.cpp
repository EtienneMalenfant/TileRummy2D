#include <gui/main_zone_components/player_tiles_container.h>

using namespace gui;

PlayerTilesContainer::PlayerTilesContainer(ITileFactory* tileFactory, const IPlayer* player)
    : _tileFactory(tileFactory), _player(player) {}

PlayerTilesContainer::~PlayerTilesContainer() {
    // delete des conteneurs
    for (int row = 0; row < _nbRow; row++) {
        delete[] _tiles[row];
    }
    delete[] _tiles;

    // delete des tuiles
    for (int i{}; i < Tile::MAX_TILE_ID; i++) {
        delete tilesById[i];
        tilesById[i] = nullptr;
    }
}

// ------------------------------
// prive

void PlayerTilesContainer::placeTile(Tile2D* tile, int row, int col) {
    sf::Vector2f basePos = Container::getPosition();
    float x = basePos.x + _leftRightPadding;
    float y = basePos.y + _topBottomPadding;
    x += col * (TileInfo::tileWidth + _colSpacing);
    y += row * (TileInfo::tileHeight + _rowSpacing);
    tile->setPosition(x, y);
    _tiles[row][col] = tile;
}

void PlayerTilesContainer::addNewTiles(const std::list<const Tile*>* playerTiles) {
    for (const Tile* tile : *playerTiles) {
        // traitement seulement pour les tuiles pas encores sur le jeu
        if (tilesById[tile->uid] == nullptr) {
            // creation de la tuile
            Tile2D* tile2D = _tileFactory->createTile(tile);
            tilesById[tile->uid] = tile2D;

            // la mettre à la fin
            int lastEmptyIndex = -1;
            // on part de la fin
            for (int i = (_nbRow * _nbCol) - 1; i >= 0; i--) {
                // garde le dernier emplacement vide
                if (_tiles[i / _nbCol][i % _nbCol] == nullptr) {
                    lastEmptyIndex = i;
                }
                // si ce n'est pas vide,
                else if (lastEmptyIndex != -1) {
                    break;
                }
            }
            placeTile(tile2D, lastEmptyIndex / _nbCol, lastEmptyIndex % _nbCol);
        }
    }
    updateLastTileIndex();
}

void PlayerTilesContainer::removePlacedTiles(const std::list<const Tile*>* playerTiles) {
    // on passe sur chaque tuiles de la grille
    for (int i = 0; i < _lastTileIndex; i++) {
        int row = i / _nbCol;
        int col = i % _nbCol;
        Tile2D* tile2d = _tiles[row][col];
        if (tile2d != nullptr) {
            // on vérifie si la tuile est toujours dans la liste des tuiles du joueur
            bool found = false;
            for (const Tile* playerTile : *playerTiles) {
                if (playerTile->uid == tile2d->getTileData()->uid) {
                    found = true;
                    break;
                }
            }
            // si la tuile n'est plus dans la liste, on la retire pour ne plus la dessiner
            if (!found) {
                _tiles[row][col] = nullptr;
            }
        }
    }
    updateLastTileIndex();
}

int PlayerTilesContainer::getRowFromPosition(const sf::Vector2f& position) {
    sf::Vector2f localPosition = position - Container::getPosition();
    // l'index correspandant à la rangée
    int row = (localPosition.y - _topBottomPadding) / (TileInfo::tileHeight + _rowSpacing);
    if (row >= _nbRow) {
        return -1;
    }
    // si le clique arrive exactement sur la hauteur de la rangée
    float localClickHeightNoSpacing = localPosition.y - _topBottomPadding - (row * _rowSpacing);
    if (localClickHeightNoSpacing <= (row + 1) * TileInfo::tileHeight && localClickHeightNoSpacing >= row * (TileInfo::tileHeight)) {
        return row;
    }
    return -1;
}

int PlayerTilesContainer::getColFromPosition(const sf::Vector2f& position) {
    sf::Vector2f localPosition = position - Container::getPosition();
    // l'index correspandant à la colonne
    int col = (localPosition.x - _leftRightPadding) / (TileInfo::tileWidth + _colSpacing);
    if (col >= _nbCol) {
        return -1;
    }
    // si le clique arrive exactement sur la largeur de la colonne
    float localClickWidthNoSpacing = localPosition.x - _leftRightPadding - (col * _colSpacing);
    if (localClickWidthNoSpacing <= (col + 1) * (TileInfo::tileWidth) && localClickWidthNoSpacing > col * (TileInfo::tileWidth)) {
        return col;
    }
    return -1;
}

int PlayerTilesContainer::getTileIndex(Tile2D* tile) {
    for (int i = 0; i <= _lastTileIndex; i++) {
        int row = i / _nbCol;
        int col = i % _nbCol;
        if (_tiles[row][col] == tile) {
            return i;
        }
    }
    return -1;
}

void PlayerTilesContainer::updateLastTileIndex() {
    for (int i = (_nbRow * _nbCol) - 1; i >= 0; i--) {
        if (_tiles[i / _nbCol][i % _nbCol] != nullptr) {
            _lastTileIndex = i;
            break;
        }
    }
}

// ------------------------------
// ITileContainer publique

Tile2D* PlayerTilesContainer::getTileAt(const sf::Vector2f& position) {

    int row = getRowFromPosition(position);
    int col = getColFromPosition(position);
    if (row < 0 || col < 0) {
        return nullptr;
    }
    if (_tiles[row][col] != nullptr) {
        if (_tiles[row][col]->getGlobalBounds().contains(position)) {
            return _tiles[row][col];
        }
    }
    return nullptr;
}

bool PlayerTilesContainer::addTileAt(Tile2D* tile, const sf::Vector2f& position) {
    int destRow = getRowFromPosition(position);
    int destCol = getColFromPosition(position);
    if (destRow >= 0 && destCol >= 0) {
        int tileIndex = getTileIndex(tile);
        int rowOrigin = tileIndex / _nbCol;
        int colOrigin = tileIndex % _nbCol;
        // si la case est déjà occupée
        if (_tiles[destRow][destCol] != nullptr) {
            // on swap les tuiles
            Tile2D* tileToSwap = _tiles[destRow][destCol];
            _tiles[rowOrigin][colOrigin] = tileToSwap;
            placeTile(tileToSwap, rowOrigin, colOrigin);
        }
        else {
            _tiles[rowOrigin][colOrigin] = nullptr;
        }
        // on met la tuile à sa nouvelle place
        _tiles[destRow][destCol] = tile;
        placeTile(tile, destRow, destCol);
        updateLastTileIndex();
        return true;
    }
    return false;
}

void PlayerTilesContainer::previewTileAt(sf::Transformable& placeholder, const sf::Vector2f& position) {
    int row = getRowFromPosition(position);
    int col = getColFromPosition(position);
    if (row >= 0 && col >= 0) {
        sf::Vector2f basePos = Container::getPosition();
        float x = basePos.x + _leftRightPadding;
        float y = basePos.y + _topBottomPadding;
        x += col * (TileInfo::tileWidth + _colSpacing);
        y += row * (TileInfo::tileHeight + _rowSpacing);
        placeholder.setPosition(x, y);
    }
    else if (row == -1 || col == -1) {
        placeholder.setPosition(-1000, -1000);
    }
}

// ------------------------------
// IUpdateable publique

void PlayerTilesContainer::update() {
    const std::list<const Tile*>* tiles = _player->getTiles();

    if (tiles->size() > lastTileCount) {
        addNewTiles(tiles);
    }
    else if (tiles->size() < lastTileCount) {
        removePlacedTiles(tiles);
    }
    // pour garder le compte des tuiles pour savoir si ajouter ou enlever une tuile
    lastTileCount = tiles->size();

    // mettre à jour la position xy des tuiles (si elles ont été déplacées dans l'écran, mais encore dans le jeu du joueur)
    for (int i = 0; i <= _lastTileIndex; i++) {
        int row = i / _nbCol;
        int col = i % _nbCol;
        if (_tiles[row][col] != nullptr) {
            placeTile(_tiles[row][col], row, col);
        }
    }
}

// ------------------------------

void PlayerTilesContainer::setSize(const sf::Vector2f& size) {
    Container::setSize(size);
    // calculer les espaces
    _nbCol = (size.x / (TileInfo::tileWidth + _colSpacing)) - 1;
    _nbRow = size.y / (TileInfo::tileHeight + _rowSpacing);
    // pour les hauteurs
    int takenSpace = (_nbRow * TileInfo::tileHeight) + ((_nbRow - 1) * _rowSpacing);
    _topBottomPadding = (size.y - takenSpace) / 2;
    // pour les largeurs
    takenSpace = (_nbCol * TileInfo::tileWidth) + ((_nbCol - 1) * _colSpacing);

    // créer le tableau de tuiles
    _tiles = new Tile2D**[_nbRow];
    for (int row = 0; row < _nbRow; row++) {
        _tiles[row] = new Tile2D*[_nbCol] {nullptr};
    }
}

void PlayerTilesContainer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    Container::draw(target, states);
    for (int i = 0; i <= _lastTileIndex; i++) {
        int row = i / _nbCol;
        int col = i % _nbCol;
        if (_tiles[row][col] != nullptr) {
            target.draw(*_tiles[row][col], states);
        }
    }
}
