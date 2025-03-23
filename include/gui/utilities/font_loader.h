#pragma once
#include <SFML/Graphics.hpp>

namespace gui {
    class FontLoader {
    public:
        static sf::Font* loadFont(const std::string& fontPath);
    };
}