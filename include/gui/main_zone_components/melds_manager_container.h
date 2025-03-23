#pragma once
#include <SFML/Graphics.hpp>
#include <gui/main_zone_components/i_tile_container.h>
#include <gui/components/i_updateable.h>
#include <game/game_dependencies.h>
#include <gui/main_zone_components/meld_containers_factory.h>

namespace gui {

    class MeldsManagerContainer : public BaseTileContainer {
    private:
        IPlayerController* _playerController;

        IMeldContainersFactory* _meldsFactory;
        std::vector<ITileContainer*> _meldsById;
        std::vector<int>** _meldsIdMap; // tableau de vecteurs de groupes de tuiles
        int _rowCapacity;

        // infos graphiques
        unsigned int _topBottomPadding = 20;
        unsigned int _leftRightPadding = TileInfo::tileWidth;
        const unsigned int _rowSpacing = 10;
        const unsigned int _colSpacing = TileInfo::tileWidth * 2; // distance entre les melds

        int getRow(const sf::Vector2f& position);
        ITileContainer* getMeldAt(const sf::Vector2f& position);
        ITileContainer* getNearMeld(const sf::Vector2f& position);

        void placeMeld(int meldContainerId);
        bool couldPlaceMeld(int row, int meldContainerId);
        float getAvailableSpace(std::vector<int>* meldList, int i, float nextXPos);
        void handleMeldOverflow(std::vector<int>* row, float lastMeldXEndPos);
        void shiftMelds(int row, int index, float lastMeldXEndPos);
        void repositionExistingMelds();
    public:
        MeldsManagerContainer(ITileFactory* tileFactory, const IMeldsContainer* meldsContainer, IPlayerController* playerController);
        ~MeldsManagerContainer() override;
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
