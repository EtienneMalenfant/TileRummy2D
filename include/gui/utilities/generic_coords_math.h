#pragma once
#include <SFML/Graphics.hpp>

namespace gui {

    class CoordsMath {
    public:
        static sf::Vector2f getOffsetToCenter(const sf::FloatRect& parentContainer, const sf::FloatRect& childContainer);
    };
}