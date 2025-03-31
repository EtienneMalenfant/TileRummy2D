#include <gui/main_zone_components/i_tile_container.h>

using namespace  gui;

gui::Tile2D* BaseTileContainer::tilesById[Tile::MAX_TILE_ID] {nullptr};

BaseTileContainer::~BaseTileContainer() {
    for (int i = 0; i < Tile::MAX_TILE_ID; ++i) {
        delete tilesById[i];
        tilesById[i] = nullptr;
    }
}
