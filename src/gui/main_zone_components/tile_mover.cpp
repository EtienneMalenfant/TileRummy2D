#include <gui/main_zone_components/tile_mover.h>

using namespace gui;

TileMover::TileMover(ITileContainer* gameContainer, ITileContainer* playerContainer) :
    _gameContainer(gameContainer),
    _playerContainer(playerContainer)
{
    _placeholder.setSize({TileInfo::tileWidth, TileInfo::tileHeight});
    _placeholder.setFillColor({255, 255, 255, 100});
}

ITileContainer* TileMover::getTargetContainer(const sf::Vector2f& mousePos) {
    if (_playerContainer->getGlobalBounds().contains(mousePos)) {
        return _playerContainer;
    } else if (_gameContainer->getGlobalBounds().contains(mousePos)) {
        return _gameContainer;
    }
    return nullptr;
}

void TileMover::handleLeftClick(const sf::Event& event) {
    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
    ITileContainer* container = getTargetContainer(mousePos);
    if (container != nullptr) {
        _selectedTile = container->getTileAt(mousePos);
        if (_selectedTile != nullptr) {
            _originalPosition = _selectedTile->getPosition();
            _clickOffset = mousePos - _selectedTile->getPosition();
            _isGameTile = container == _gameContainer;
        }
    }
}

void TileMover::handleLeftRelease(const sf::Event& event) {
    if (_selectedTile == nullptr) {
        return;
    }
    _placeholder.setPosition({-500, -500});
    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
    bool hasPlacedTile = false;
    ITileContainer* targetContainer = getTargetContainer(mousePos);
    if (targetContainer != nullptr) {
        // s'assurer qu'on ne place pas une tuile du jeu sur notre jeu
        if ((targetContainer == _playerContainer && _isGameTile) == false) {
            hasPlacedTile = targetContainer->addTileAt(_selectedTile, mousePos);
        }
        if (hasPlacedTile) {
            static_cast<IUpdateable*>(_gameContainer)->update(); // cast parce que update() est aussi sur sf::Shape
        }
    }
    if (hasPlacedTile == false) {
        _selectedTile->setPosition(_originalPosition);
    }
    _selectedTile = nullptr;
}

void TileMover::handle(const sf::Event& event) {
    // les cliques
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            handleLeftClick(event);
        }
    }
    // les relâchements
    else if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left && _selectedTile != nullptr) {
            handleLeftRelease(event);
        }
    } // les déplacements
    else if (event.type == sf::Event::MouseMoved) {
        if (_selectedTile != nullptr) {
            sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
            // pour pas que la tuile sorte de la fenêtre
            ITileContainer* targetContainer = getTargetContainer(mousePos);
            if (targetContainer != nullptr) {
                _selectedTile->setPosition(mousePos - _clickOffset);
                targetContainer->previewTileAt(_placeholder, mousePos);
            }
        }
    }
}

void TileMover::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (_selectedTile != nullptr) {
        target.draw(_placeholder, states);
        target.draw(*_selectedTile, states);
    }
}
