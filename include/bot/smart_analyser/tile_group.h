#pragma once
#include <list>
#include <game/action.h>
#include <bot/smart_analyser/tile_node.h>

namespace bot {

    class ITileGroup {
        public:
            virtual bool contains(ITileNode* tileNode) = 0;
            virtual int getPlayerTileCount() = 0;
            virtual std::list<Action*>* toActions() = 0;
            virtual ~ITileGroup() = default;
        };

        // cette classe à pour but d'optimiser les insertions en ne faisaint pas une actions
        // pour chaque tuile du meld existant
        class InsertionGroup : public ITileGroup {
        private:
            ITileNode* _insertedTile;
            ITileNode* _rightNeighbour;
            ITileNode* _leftNeighbour;
        public:
            InsertionGroup(ITileNode* insertedTile, ITileNode* leftNeighbour, ITileNode* rightNeighbour);
            bool contains(ITileNode* tileNode) override;
            int getPlayerTileCount() override;
            std::list<Action*>* toActions() override;
        };

        class MeldRebuildGroup : public ITileGroup {
        private:
            std::vector<ITileNode*> _meldNodes;
        public:
            MeldRebuildGroup(std::vector<ITileNode*> meldNodes);
            bool contains(ITileNode* tileNode) override;
            int getPlayerTileCount() override;
            std::list<Action*>* toActions() override;
        };
}
