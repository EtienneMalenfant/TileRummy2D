#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

namespace gui {
    class FpsCounter : public sf::Drawable {
    private:
        sf::Font* _font;
        mutable sf::Clock _clock;
        mutable sf::Text _text;
        mutable int _frameCount = 0;

    public:
        FpsCounter();
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };
}