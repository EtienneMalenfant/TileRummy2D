#pragma once
#include <vector>
#include <list>
#include <array>
#include <bot/smart_analyser/tile_wrapper.h>
#include <game/melds_manager.h>

namespace bot {
     // S'occupe de donner les TileWrapper compatible avec la tuile donnée
     class ITileWrapperManager {
        public:
            virtual void refresh(const std::list<const Tile*>* playerTiles) = 0;
            virtual const std::vector<TileWrapper*>* getCompatibleTiles(const Tile* tile) = 0;
            virtual const std::vector<TileWrapper*>* getPlayerTiles() = 0;
            virtual ~ITileWrapperManager() = default;
        };

        class TileWrapperManager : public ITileWrapperManager {
        private:
            IMeldsContainer* _meldsContainer;

            TileWrapper* _tiles[4][13][2] {nullptr};
            TileWrapper* _jokers[2] {nullptr};
            std::vector<TileWrapper*> _playerTiles;

            void resetArrays();
            void fillGameTiles();

            void addWrapper(TileWrapper* tileWrapper);
            std::array<TileWrapper*,2> getWrappers(Color color, int value);

            std::vector<TileWrapper*>* getSetCompatibleTiles(Color color, int value);
            std::vector<TileWrapper*>* getSequenceCompatibleTiles(Color color, int value);

        public:
            TileWrapperManager(IMeldsContainer* meldsContainer);
            ~TileWrapperManager();

            void refresh(const std::list<const Tile*>* playerTiles) override;
            const std::vector<TileWrapper*>* getPlayerTiles() override;
            const std::vector<TileWrapper*>* getCompatibleTiles(const Tile* tile) override;
        };
}
