#pragma once
#include <SFML/Graphics.hpp>

namespace gui {

    enum class Font {
        Inconsolata,
        Inter
    };

    class FontLoader {
    private:
        static std::string getFontPath(Font font);
    public:
        static sf::Font* loadFont(Font font);
    };
}
