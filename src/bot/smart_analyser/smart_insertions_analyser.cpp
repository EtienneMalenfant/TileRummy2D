#include <bot/smart_analyser/smart_insertions_analyser.h>

using namespace bot;

SmartInsertionsAnalyser::SmartInsertionsAnalyser(IMeldsContainer* meldsContainer) {
    _tileWrapperManager = new TileWrapperManager(meldsContainer);
    _tileGraph = new TileGraph();
    _meldBuilder = new MeldBuilder();
}

SmartInsertionsAnalyser::~SmartInsertionsAnalyser() {
    delete _tileWrapperManager;
    delete _tileGraph;
    delete _meldBuilder;
}

std::list<std::list<Action*>*>* SmartInsertionsAnalyser::getActionsSequences(const std::list<const Tile*>* playerTiles, bool useJoker) {
    _tileWrapperManager->refresh(playerTiles);
    _tileGraph->build(_tileWrapperManager);
    IGameBuildState* gameState = _meldBuilder->buildGame(_tileGraph, useJoker);
    std::list<std::list<Action*>*>* actionsLists = gameState->toActions();
    return actionsLists;
}
