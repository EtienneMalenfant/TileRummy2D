#include <gui/info_zone_components/game_info_container.h>

using namespace gui;

GameInfosContainer::GameInfosContainer(Container* gameLogsComponent, Container* playerInfosComponent) :
    _gameLogsComponent(gameLogsComponent),
    _playerInfosComponent(playerInfosComponent) {}

GameInfosContainer::~GameInfosContainer() {
    delete _gameLogsComponent;
    delete _playerInfosComponent;
}


void GameInfosContainer::setPosition(float x, float y) {
    Container::setPosition(x, y);
    _gameLogsComponent->setPosition(x, y);
    _playerInfosComponent->setPosition(x, y + _gameLogsComponent->getLocalBounds().height);
}

void GameInfosContainer::setPosition(const sf::Vector2f& position) {
    setPosition(position.x, position.y);
}

void GameInfosContainer::setSize(const sf::Vector2f& size) {
    Container::setSize(size);
    _gameLogsComponent->setSize(sf::Vector2f(size.x, size.y * LOG_SECTION_HEIGHR_RATIO));
    _playerInfosComponent->setSize(sf::Vector2f(size.x, size.y * (1 - LOG_SECTION_HEIGHR_RATIO)));
}

void GameInfosContainer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    Container::draw(target, states);
    _gameLogsComponent->draw(target, states);
    _playerInfosComponent->draw(target, states);
}