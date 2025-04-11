#pragma once
#include <SFML/Graphics.hpp>

#include <gui/components/container.h>

namespace gui {

    class Button : public Container {
    private:
        sf::Text _name;
    public:
        Button(const std::string& name, sf::Font* font, const sf::Color& textColor = sf::Color::Black);
        std::string getName() const;

        // Container
        void setPosition(const sf::Vector2f& position) override;
        void setPosition(float x, float y) override;
        void setSize(const sf::Vector2f& size) override;
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };

}
