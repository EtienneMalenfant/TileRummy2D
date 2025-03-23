#pragma once
#include <gui/main_zone_components/i_tile_container.h>
#include <gui/events/i_window_event_handler.h>

namespace gui {

    class TileMover : public IWindowEventHandler, public sf::Drawable {
    private:
        ITileContainer* _gameContainer;
        ITileContainer* _playerContainer;

        Tile2D* _selectedTile { nullptr };
        sf::RectangleShape _placeholder;
        bool _isGameTile { false };
        sf::Vector2f _originalPosition;
        sf::Vector2f _clickOffset;

        ITileContainer* getTargetContainer(const sf::Vector2f& mousePos);
        void handleLeftClick(const sf::Event& event);
        void handleLeftRelease(const sf::Event& event);
    public:
        TileMover(ITileContainer* gameContainer, ITileContainer* playerContainer);
        ~TileMover() override = default;

        void handle(const sf::Event& event) override;

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };
}
