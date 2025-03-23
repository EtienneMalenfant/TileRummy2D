#pragma once
#include <bot/smart_analyser/tile_graph.h>
#include <bot/smart_analyser/game_build_state.h>
#include <stack>

namespace bot {

    class IMeldBuilder {
    public:
        virtual IGameBuildState* buildGame(ITileGraph* tilGraph, bool useJoker) = 0;

        virtual ~IMeldBuilder() = default;
    };

    class MeldBuilder : public IMeldBuilder {
    private:
        ITileGraph* _tileGraph;
        IGameBuildState* _currentGameState {nullptr};
        bool _useJoker = false;
        const int _maxRecursionLevel = 1;

        //définition dans meld_builder_extends.cpp
        bool extendSet(ITileNode* node);
        bool extendSequence(ITileNode* node);

        std::vector<ITileNode*>* copyCompatibleNodes(ITileNode* node, ValueDirection d);
        bool meldContainTile(ITileNode* searchedNode, ITileNode* meldNode);

        std::vector<ITileNode*>* getSequenceExtensions(ITileNode* node, ValueDirection direction);
        std::vector<ITileNode*>* getOrphansFromRemoval(ITileNode* node, MeldType meldType);
        bool replaceOrphanedTiles(std::vector<ITileNode*>* sequenceExtension, ITileNode* removedNode, int recursionLevel);
        void filterJokerNeighbors(std::vector<ITileNode*>* compatibleNodes, ITileNode* node, ValueDirection direction);
        std::vector<ITileNode*>* getFilteredCompatibleNodes(std::vector<ITileNode*>& currentMeldBuild, ITileNode* node, ValueDirection direction);
        void orderCompatibleNodes(std::vector<ITileNode*>* compatibleNodes, ITileNode* node);
        bool formSet(ITileNode* node, std::vector<ITileNode*>& currentMeldBuild, int recursionLevel);
        bool formSequence(ITileNode* node, std::vector<ITileNode*>& currentMeldBuild, std::stack<ICommand*>* linkNodeCommands,
            ValueDirection direction, int recursionLevel, bool isBuildingBothWays);
        std::vector<MeldType> getMeldTypeOrder(ITileNode* node);
        bool organizeNodePlacement(ITileNode* node, int recursionLevel);
    public:
        MeldBuilder() = default;
        ~MeldBuilder() override = default;

        IGameBuildState* buildGame(ITileGraph* tilGraph, bool useJoker) override;
    };
}
