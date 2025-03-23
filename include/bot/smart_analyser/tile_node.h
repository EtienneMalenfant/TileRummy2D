#pragma once
#include <bot/smart_analyser/tile_wrapper.h>
#include <bot/smart_analyser/tile_node_iterator.h>
#include <array>

namespace bot {

    enum class MeldDirection {
        Previous,
        Next
    };

    enum class ValueDirection {
        Lower,
        Same,
        Higher
    };

    namespace EnumConversion {
        MeldDirection getMeldDirection(ValueDirection d);
    };

    class ITileNodeBuildTools {
    public:
        virtual void addCompatibleNode(ITileNode* tileNode) = 0;
        virtual bool hasNeighbour(ITileNode* tileNode) = 0;
    };

    class ITileNode : public ITileNodeBuildTools {
    public:
        virtual void linkMeldTile(ITileNode* node, MeldDirection d) = 0;
        virtual ITileNode* unlinkMeldTile( MeldDirection d) = 0;
        virtual int getMeldTileCount() = 0;
        virtual int getMeldTileCount(MeldDirection d) = 0;
        virtual ITileNode* getMeldTile(MeldDirection d) = 0;

        virtual TileWrapper* getTileWrapper() = 0;
        virtual const std::vector<ITileNode*>& getCompatibleNodes(ValueDirection d) = 0;
        virtual Iterator<ITileNode*>* getMeldIterator() = 0;
        virtual MeldType evaluateMeldType() = 0;

        virtual ~ITileNode() = default;
    };

    class TileNode : public ITileNode {
    private:
        TileWrapper* _tileWrapper;

        TileNode* _meldNext {nullptr};
        TileNode* _meldPrevious {nullptr};

        std::vector<ITileNode*> _lowerValueNeighbours;
        std::vector<ITileNode*> _sameValueNeighbours;
        std::vector<ITileNode*> _higherValueNeighbours;
        std::array<std::vector<ITileNode*>*, 3> _allNeighbours;

        void buildIterator(std::vector<ITileNode*>* meldTiles, MeldDirection d);
        void addMeldTile(ITileNode* node);
        bool hasNeighbour(ITileNode* tileNode) override;
    public:
        TileNode(TileWrapper* tileWrapper);
        ~TileNode() override = default;

        void addCompatibleNode(ITileNode* tileNode) override;
        void linkMeldTile(ITileNode* node, MeldDirection d) override;

        ITileNode* unlinkMeldTile(MeldDirection d) override;
        int getMeldTileCount() override;
        int getMeldTileCount(MeldDirection d) override;
        ITileNode* getMeldTile(MeldDirection d);

        TileWrapper* getTileWrapper() override;
        const std::vector<ITileNode*>& getCompatibleNodes(ValueDirection d) override;
        Iterator<ITileNode*>* getMeldIterator() override;
        MeldType evaluateMeldType() override;
    };
};
