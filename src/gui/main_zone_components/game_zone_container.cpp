#include <gui/main_zone_components/game_zone_container.h>

using namespace gui;

GameZoneContainer::GameZoneContainer(Container* playerTilesContainer, Container* meldsContainer, sf::Drawable* tileMover, float mainZoneHeightRatio)
    : _playerTilesContainer(playerTilesContainer), _meldsContainer(meldsContainer), _mainZoneHeightRatio(mainZoneHeightRatio), _tileMover(tileMover) {}

GameZoneContainer::~GameZoneContainer() {
    delete _playerTilesContainer;
    delete _meldsContainer;
}

void GameZoneContainer::setPosition(float x, float y) {
    Container::setPosition(x, y);
    _meldsContainer->setPosition(x, y);
    _playerTilesContainer->setPosition(x, y + _meldsContainer->getLocalBounds().height);
}

void GameZoneContainer::setPosition(const sf::Vector2f& position) {
    setPosition(position.x, position.y);
}

void GameZoneContainer::setSize(const sf::Vector2f& size) {
    Container::setSize(size);
    _meldsContainer->setSize({size.x, size.y * _mainZoneHeightRatio});
    _playerTilesContainer->setSize({size.x, size.y * (1 - _mainZoneHeightRatio)});
}

void GameZoneContainer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(*_meldsContainer, states);
    target.draw(*_playerTilesContainer, states);
    target.draw(*_tileMover, states);
}