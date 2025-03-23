#pragma once
#include <gui/main_zone_components/tile.h>

namespace gui {

    class ITileFactory {
    public:
        virtual Tile2D* createTile(const Tile* const tile) = 0;
        virtual ~ITileFactory() = default;
    };

    class TileFactory : public ITileFactory {
    private:
        const TileInfo* _tileInfo;
        sf::Color colors[4] = {sf::Color::Black};
    public:
        TileFactory(const TileInfo* numberTileInfo);
        ~TileFactory() override;
        Tile2D* createTile(const Tile* const tile) override;
    };
}