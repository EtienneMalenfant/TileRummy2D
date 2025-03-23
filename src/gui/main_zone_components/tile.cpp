#include <gui/main_zone_components/tile.h>
#include <gui/utilities/generic_coords_math.h>

using namespace gui;


Tile2D::Tile2D(const Tile* const tile, const TileInfo& infos)
: Container({TileInfo::tileWidth, TileInfo::tileHeight}, infos.tileColor), _tile(tile) {
    int colorOffset = 20;
    sf::Color outlineColor = {static_cast<sf::Uint8>(infos.tileColor.r - colorOffset),
        static_cast<sf::Uint8>(infos.tileColor.g - colorOffset), static_cast<sf::Uint8>(infos.tileColor.b - colorOffset)};
    Container::setOutlineColor(outlineColor);
    Container::setOutlineThickness(-2);

    // DEBUG ONLY
    _id.setFont(*infos.font);
    _id.setCharacterSize(13);
    _id.setFillColor(sf::Color::Black);
    _id.setString(std::to_string(tile->uid));
}

Tile2D::~Tile2D() {}

const Tile* Tile2D::getTileData() const {
    return _tile;
}

// ------------------------------

NumberTile::NumberTile(const Tile* const tile, const TileInfo& infos, const sf::Color& color) : Tile2D(tile, infos)  {
    _valueText.setFont(*infos.font);
    _valueText.setCharacterSize(infos.valueSize);
    _valueText.setFillColor(color);
    _valueText.setString(std::to_string(tile->value));
}

NumberTile::~NumberTile() {}

void NumberTile::setPosition(float x, float y) {
    Container::setPosition(x, y);
    sf::Vector2f offset = CoordsMath::getOffsetToCenter(Container::getLocalBounds(), _valueText.getLocalBounds());
    _valueText.setPosition(x + offset.x, y + offset.y);
    _id.setPosition(x, y); // DEBUG ONLY
}

void NumberTile::setPosition(const sf::Vector2f& position) {
    setPosition(position.x, position.y);
}

void NumberTile::setSize(const sf::Vector2f& size) {
    // TODO : update la grosseur du nombre
    Container::setSize(size);
}

void NumberTile::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    Container::draw(target, states);
    target.draw(_valueText, states);
    //target.draw(_id, states); // DEBUG ONLY
}

// ------------------------------

JokerTile::JokerTile(const Tile* const tile, const TileInfo& infos, const sf::Color& color) : Tile2D(tile, infos) {
    _jokerSign.setString("*");
    _jokerSign.setFont(*infos.font);
    _jokerSign.setCharacterSize(infos.jokerSize);
    _jokerSign.setFillColor(color);
}

JokerTile::~JokerTile() {}

void JokerTile::setPosition(float x, float y) {
    Container::setPosition(x, y);
    sf::Vector2f offset = CoordsMath::getOffsetToCenter(Container::getLocalBounds(), _jokerSign.getLocalBounds());
    _jokerSign.setPosition(x + offset.x, y + offset.y);
    _id.setPosition(x, y); // DEBUG ONLY
}

void JokerTile::setPosition(const sf::Vector2f& position) {
    setPosition(position.x, position.y);
}

void JokerTile::setSize(const sf::Vector2f& size) {
    // TODO : update la grosseur du joker
    Container::setSize(size);
}

void JokerTile::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    Container::draw(target, states);
    target.draw(_jokerSign, states);
    // target.draw(_id, states); // DEBUG ONLY
}

