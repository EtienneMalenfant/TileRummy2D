#pragma once
#include <list>
#include <vector>
#include <gui/main_zone_components/i_tile_container.h>
#include <gui/main_zone_components/meld_container.h>
#include <game/meld.h>
#include <game/player.h>

namespace gui {
    class IMeldContainersFactory {
        public:
        virtual void createNewMeldContainers() = 0;
        virtual void deleteOldMeldContainers(std::vector<int>** meldsIdMap, int meldMapSize) = 0;
            virtual ~IMeldContainersFactory() = default;
        };
    
        class MeldContainersFactory : public IMeldContainersFactory {
        private:
            ITileFactory* _tileFactory;
            IPlayerController* _playerController;
            // les melds du jeu
            const std::list<const IMeld*>* _gameMelds;
            // les containers de melds, classés par id
            std::vector<ITileContainer*>* _meldsById;
            std::vector<bool> _gameHasMeld;
    
            void resetBoolArray();
        public:
            MeldContainersFactory(const std::list<const IMeld*>* gameMelds, std::vector<ITileContainer*>* meldsById, ITileFactory* tileFactory, IPlayerController* playerController);
            void createNewMeldContainers() override;
            void deleteOldMeldContainers(std::vector<int>** meldsIdMap, int meldMapSize) override;
        };
}