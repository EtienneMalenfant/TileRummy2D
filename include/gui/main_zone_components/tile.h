#pragma once
#include <SFML/Graphics.hpp>
#include <game/tile.h>
#include <gui/components/container.h>

namespace gui {

    struct TileInfo {
    public:
        const sf::Color tileColor;
        const int valueSize;
        const int jokerSize;
        const sf::Font* font;
        const static int tileWidth = 55;
        const static int tileHeight = 80;
    };

    class Tile2D : public Container {
    private:
        const Tile* const _tile; // game/tile.h
#ifdef DEBUG
    sf::Text _id;
#endif

    public:
        Tile2D(const Tile* const tile, const TileInfo& tileInfo);
        ~Tile2D() override;
        const Tile* getTileData() const;
        virtual void setPosition(float x, float y);
        virtual void setPosition(const sf::Vector2f& position);
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    };

    class NumberTile : public Tile2D {
    private:
        sf::Text _valueText;
    public:
        NumberTile(const Tile* const tile, const TileInfo& infos, const sf::Color& color);
        ~NumberTile() override;
        void setPosition(float x, float y);
        void setPosition(const sf::Vector2f& position);
        void setSize(const sf::Vector2f& size);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };

    class JokerTile : public Tile2D {
    private:
        sf::Text _jokerSign; // Pourrait etre un dessin dans le future
    public:
        // TODO : changer les paramètres
        JokerTile(const Tile* const tile, const TileInfo& infos, const sf::Color& color);
        ~JokerTile() override;
        void setPosition(float x, float y);
        void setPosition(const sf::Vector2f& position);
        void setSize(const sf::Vector2f& size);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };

}
