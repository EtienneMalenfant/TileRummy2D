#pragma once
#include <string>

enum class Color {
    RED,
    YELLOW,
    BLUE,
    BLACK
};

// ------------------------------

class Tile {
private:
    int _value;
    Color _color;

    void validateValue();
public:
    const int& value {_value};
    const Color& color {_color};
    const bool isJoker;
    const int uid;

    static constexpr int MAX_TILE_ID = 106;
    static int nextTileUID;

    Tile(int value, Color color, bool isJoker);
    Tile(int value, Color color, bool isJoker, int uid); // pour les tests seulement
    void setValue(int); //seulement valide pour les jokers
    void setColor(Color);

    ~Tile() = default;
};
