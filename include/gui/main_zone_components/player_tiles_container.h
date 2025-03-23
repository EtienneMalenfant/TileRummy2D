#pragma once
#include <gui/main_zone_components/i_tile_container.h>
#include <gui/components/i_updateable.h>
#include <game/player.h>

namespace gui {

    class PlayerTilesContainer : public BaseTileContainer {
    private:
        const IPlayer* _player;
        ITileFactory* _tileFactory;
        Tile2D*** _tiles {};
        int lastTileCount = 0;

        // données graphiques
        int _topBottomPadding = 10;
        int _leftRightPadding = 30;
        const int _rowSpacing = 7;
        const int _colSpacing = 0;
        //capacité
        int _nbRow;
        int _nbCol;
        //taille actuelle
        int _lastTileIndex = 0;

        void placeTile(Tile2D* tile, int row, int col);
        void addNewTiles(const std::list<const Tile*>* playerTiles);
        void removePlacedTiles(const std::list<const Tile*>* playerTiles);
        int getRowFromPosition(const sf::Vector2f& position);
        int getColFromPosition(const sf::Vector2f& position);
        int getTileIndex(Tile2D* tile);
        void updateLastTileIndex();
    public:
        PlayerTilesContainer(ITileFactory* tileFactory, const IPlayer* player);
        ~PlayerTilesContainer() override;

        // ITileContainer
        Tile2D* getTileAt(const sf::Vector2f& position) override;
        bool addTileAt(Tile2D* tile, const sf::Vector2f& position) override;
        void previewTileAt(sf::Transformable& placeholder, const sf::Vector2f& position) override;

        // IUpdateable
        void update() override;

        // sf::Shape
        void setSize(const sf::Vector2f& size) override;
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };
}
