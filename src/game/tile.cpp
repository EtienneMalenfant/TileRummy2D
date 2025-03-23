#include <game/tile.h>
#include <stdexcept>

int Tile::nextTileUID = 0;

Tile::Tile(int value, Color color, bool isJoker) :
    _value(value), _color(color), isJoker(isJoker), uid(Tile::nextTileUID) 
{
    nextTileUID++;
    validateValue();
}

Tile::Tile(int value, Color color, bool isJoker, int uid) :
    _value(value), _color(color), isJoker(isJoker), uid(uid) 
{
    validateValue();
}

void Tile::validateValue() {
    if ((_value <= 0 || _value > 13) && !isJoker) {
        throw std::invalid_argument("La valeur doit être entre 0 et 13");
    }
}

void Tile::setValue(int newValue) {
    if (isJoker) {
        _value = newValue;
    }
}

void Tile::setColor(Color newColor) {
    if (isJoker) {
        _color = newColor;
    }
}