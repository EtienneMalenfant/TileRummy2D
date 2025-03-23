#pragma once
#include <SFML/Graphics.hpp>

namespace gui {

    class IComponentBuilder {
    public:
        virtual void setEmplacement(const sf::FloatRect& emplacement) = 0;
        virtual sf::Drawable* getDrawable() = 0;
        virtual ~IComponentBuilder() = default;
    };
}