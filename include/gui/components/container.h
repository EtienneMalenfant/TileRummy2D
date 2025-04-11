#pragma once
#include <SFML/Graphics.hpp>

class Container : public sf::Shape {
private:
    sf::RectangleShape _background;
public:
    Container(const sf::Vector2f& size, const sf::Color& color);
    Container() = default;
    // sf::Shape
    virtual std::size_t getPointCount() const override;
    virtual sf::Vector2f getPoint(std::size_t index) const override;
    virtual sf::FloatRect getLocalBounds() const;
    virtual sf::FloatRect getGlobalBounds() const;
    virtual void setPosition(const sf::Vector2f& position);
    virtual void setPosition(float x, float y);
    virtual void setFillColor(const sf::Color& color);
    virtual void setOutlineColor(const sf::Color& color);
    virtual void setOutlineThickness(float thickness);
    virtual const sf::Vector2f& getPosition() const;
    virtual const sf::Vector2f& getSize() const;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    // Méthode ajoutée
    virtual void setSize(const sf::Vector2f& size);
};
