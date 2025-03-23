#include <gui/components/container.h>

Container::Container(const sf::Vector2f& size, const sf::Color& color) {
    _background.setSize(size);
    _background.setFillColor(color);
}

std::size_t Container::getPointCount() const {
    return _background.getPointCount();
}

sf::Vector2f Container::getPoint(std::size_t index) const {
    return _background.getPoint(index);
}

sf::FloatRect Container::getLocalBounds() const {
    return _background.getLocalBounds();
}

sf::FloatRect Container::getGlobalBounds() const {
    return _background.getGlobalBounds();
}

void Container::setPosition(const sf::Vector2f& position) {
    _background.setPosition(position);
}

void Container::setPosition(float x, float y) {
    _background.setPosition(x, y);
}

void Container::setFillColor(const sf::Color& color) {
    _background.setFillColor(color);
}

void Container::setOutlineColor(const sf::Color& color) {
    _background.setOutlineColor(color);
}

void Container::setOutlineThickness(float thickness) {
    _background.setOutlineThickness(thickness);
}

const sf::Vector2f& Container::getPosition() const {
    return _background.getPosition();
}

const sf::Vector2f& Container::getSize() const {
    return _background.getSize();
}

void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(_background, states);
}

void Container::setSize(const sf::Vector2f& size) {
    _background.setSize(size);
}

