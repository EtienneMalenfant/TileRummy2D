#pragma once
#include <bot/smart_analyser/tile_wrapper_manager.h>
#include <bot/smart_analyser/tile_node.h>

namespace bot {

    class ITileGraph {
    public:
        virtual void build(ITileWrapperManager*) = 0;
        virtual const std::vector<ITileNode*>& getPlayerTileNodes() = 0;

        virtual ~ITileGraph() = default;
    };

    class TileGraph : public ITileGraph {
    private:
        std::vector<ITileNode*> _playerTileNodes;
        std::vector<ITileNode*> _allNodes;
        ITileWrapperManager* _tileWrapperManager;

        void reset();
        ITileNode* getNode(TileWrapper* tileWrapper);
        void linkCompatibleTiles(ITileNode* node, const Tile* tile);
    public:
        TileGraph() = default;
        ~TileGraph() override;
        void build(ITileWrapperManager*) override;
        const std::vector<ITileNode*>& getPlayerTileNodes();
    };
};
