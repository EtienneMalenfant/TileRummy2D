#pragma once
#include <gui/main_zone_components/tile_container.h>
#include <gui/window_events.h>

namespace gui
{
    class TileMover : public IEventHandler {
    private:
        ITileContainer* _playerTilesContainer;
        ITileContainer* _meldsContainer;

        sf::Vector2f _offset;
        sf::Vector2f _origin;
        Tile2D* _selectedTile;
    public:
        TileMover(ITileContainer* playerTilesContainer, ITileContainer* meldsContainer);
        ~TileMover() override = default;
        void handle(const sf::Event& event) override;
    };
} 
