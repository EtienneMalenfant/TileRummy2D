#include <gui/main_zone_components/tile_factory.h>

using namespace gui;

TileFactory::TileFactory(const TileInfo* tileInfo) : _tileInfo(tileInfo) {
    colors[(int)Color::RED] = sf::Color::Red;
    colors[(int)Color::YELLOW] = sf::Color::Yellow;
    colors[(int)Color::BLUE] = sf::Color::Blue;
    colors[(int)Color::BLACK] = sf::Color::Black;
}

TileFactory::~TileFactory() {
    delete _tileInfo;
}

Tile2D* TileFactory::createTile(const Tile* const tile) {
    if (tile->isJoker) {
        return new JokerTile(tile, *_tileInfo, colors[(int)tile->color]);
    }
    return new NumberTile(tile, *_tileInfo, colors[(int)tile->color]);
}