#pragma once
#include <SFML/Graphics.hpp>
#include <gui/main_zone_components/tile.h>
#include <gui/main_zone_components/tile_factory.h>
#include <gui/components/container.h>
#include <gui/components/i_updateable.h>

namespace gui {

    class ITileContainer : public Container, public IUpdateable {
    public:
        virtual Tile2D* getTileAt(const sf::Vector2f& position) = 0;
        virtual bool addTileAt(Tile2D* tile, const sf::Vector2f& position) = 0;
        virtual void previewTileAt(sf::Transformable& placeholder, const sf::Vector2f& position) = 0;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

        virtual ~ITileContainer() = default;
    };


    // Contient tous les tuiles visuelles à l'écran
    class BaseTileContainer : public ITileContainer {
    protected:
        static Tile2D* tilesById[Tile::MAX_TILE_ID];
    };
}
