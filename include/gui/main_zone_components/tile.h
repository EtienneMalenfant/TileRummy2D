#pragma once
#include <SFML/Graphics.hpp>
#include <game/tile.h>
#include <gui/components/container.h>

namespace gui {

    struct TileInfo {
    public:
        const sf::Color tileColor;
        const u_int valueSize;
        const u_int jokerSize;
        const sf::Font* font;
        const static u_int tileWidth = 55;
        const static u_int tileHeight = 80;
    };

    class Tile2D : public Container {
    private:
        const Tile* const _tile; // game/tile.h

    protected:
        sf::Text _id; // ! debug only
    public:
        Tile2D(const Tile* const tile, const TileInfo& tileInfo);
        ~Tile2D() override;
        const Tile* getTileData() const;
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
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