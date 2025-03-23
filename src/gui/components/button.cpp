#include <gui/components/button.h>
#include <gui/utilities/generic_coords_math.h>

using namespace gui;

Button::Button(const std::string& name, sf::Font* font, const sf::Color& textColor) {
    _name.setFont(*font);
    _name.setString(name);
    _name.setCharacterSize(14);
    _name.setFillColor(textColor);
}

std::string Button::getName() const {
    return _name.getString();
}

// ------------------------------

void Button::setPosition(float x, float y) {
    Container::setPosition(x, y);
    sf::Vector2f offset = CoordsMath::getOffsetToCenter(Container::getLocalBounds(), _name.getLocalBounds());
    _name.setPosition(x + offset.x, y + offset.y);
}

void Button::setPosition(const sf::Vector2f& position) {
    setPosition(position.x, position.y);
}

void Button::setSize(const sf::Vector2f& size) {
    Container::setSize(size);
    _name.setCharacterSize(size.y / 2);
    _name.setPosition(Container::getPosition());
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    Container::draw(target, states);
    target.draw(_name, states);
}