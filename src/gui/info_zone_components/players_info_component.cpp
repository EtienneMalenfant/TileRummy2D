#include <gui/info_zone_components/players_info_component.h>

using namespace gui;

PlayersInfoComponent::PlayersInfoComponent(const std::vector<IPlayer*>* const players, const sf::Font* font) :
    _players(players),
    _font(font),
    _nbPlayers(players->size())
{
    for (int i = 0; i < _nbPlayers; i++) {
        _playersInfo[i].setFont(*_font);
        _playersInfo[i].setCharacterSize(_fontSize);
        _playersInfo[i].setFillColor(sf::Color::White);
    }
}

void PlayersInfoComponent::updatePosition() {
    sf::FloatRect bounds = Container::getGlobalBounds();
    float x = bounds.left + 10;
    float y = bounds.top + 10;
    for (int i = 0; i < _nbPlayers; i++) {
        _playersInfo[i].setPosition(x, y);
        y += _fontSize + 10;
    }
}

void PlayersInfoComponent::setPosition(float x, float y) {
    Container::setPosition(x, y);
    updatePosition();
}

void PlayersInfoComponent::setPosition(const sf::Vector2f& position) {
    Container::setPosition(position);
    updatePosition();
}

void PlayersInfoComponent::update() {
    for (int i = 0; i < _nbPlayers; i++) {
        IPlayer* player = _players->at(i);
        std::string playerName = player->getName();
        playerName = playerName.substr(0, 12); // couper le nom à 12 caractères
        playerName = playerName.append(std::max(12 - (int)playerName.size(), 0), ' ') + " : "; // nom + espace pour aligner
        _playersInfo[i].setString(playerName + std::to_string(player->getTiles()->size()) + " tuiles"); // nom + nombre de tuiles
    }
}

void PlayersInfoComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (int i = 0; i < _nbPlayers; i++) {
        target.draw(_playersInfo[i], states);
    }
}
