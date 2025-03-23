#pragma once
#include <gui/main_zone_components/i_tile_container.h>
#include <gui/components/i_updateable.h>
#include <game/meld.h>
#include <game/player.h>

namespace gui {

    class MeldContainer : public BaseTileContainer {
    private:
        ITileFactory* _tileFactory;
        std::vector<Tile2D*> _tiles;
        const IMeld* _meld;
        static IPlayerController* _playerController;

        void updateContainerSize();
        void updateTilesPosition();
        void placeTile(const Tile* tileData, int index);
    public:
        MeldContainer(const IMeld* meld, ITileFactory* tileFactory, IPlayerController* playerController);
        ~MeldContainer() override;
        // ITileContainer
        Tile2D* getTileAt(const sf::Vector2f& position) override;
        bool addTileAt(Tile2D* tile, const sf::Vector2f& position) override;
        void previewTileAt(sf::Transformable& placeholder, const sf::Vector2f& position) override;

        // sf::Shape
        void setPosition(float x, float y) override;
        void setPosition(const sf::Vector2f& position) override;
        void setFillColor(const sf::Color& color) override;
        void setSize(const sf::Vector2f& size) override;
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        // IUpdateable
        void update() override;
    };
}
