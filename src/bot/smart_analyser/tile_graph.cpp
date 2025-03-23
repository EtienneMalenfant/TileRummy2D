#include <bot/smart_analyser/tile_graph.h>

using namespace bot;

TileGraph::~TileGraph() {
    reset();
}

void TileGraph::reset() {
    for (ITileNode* node : _allNodes) {
        delete node;
    }
    _allNodes.clear();
    _playerTileNodes.clear();
}

ITileNode* TileGraph::getNode(TileWrapper* tileWrapper) {
    // rechercher si le noeud existe déjà
    for (ITileNode* node : _allNodes) {
        if (node->getTileWrapper() == tileWrapper) {
            return node;
        }
    }
    ITileNode* newNode = new TileNode(tileWrapper);
    _allNodes.push_back(newNode);
    return newNode;
}

// méthode récursive pour assembler les tuiles compatibles
void TileGraph::linkCompatibleTiles(ITileNode* previousNode, const Tile* tile) {
    const std::vector<TileWrapper*>* compatibleTiles = _tileWrapperManager->getCompatibleTiles(tile);
    for (TileWrapper* tileWrapper : *compatibleTiles) {
        ITileNode* newNode = getNode(tileWrapper);
        if (previousNode->hasNeighbour(newNode) == false) {
            previousNode->addCompatibleNode(newNode);
            linkCompatibleTiles(newNode, tileWrapper->tile);
        }
    }
    delete compatibleTiles;
}


void TileGraph::build(ITileWrapperManager* wrapperManager) {
    _tileWrapperManager = wrapperManager;
    reset();
    const std::vector<TileWrapper*>* playerTiles = _tileWrapperManager->getPlayerTiles();
    for (TileWrapper* playerTile : *playerTiles) {
        ITileNode* tileNode = getNode(playerTile);
        _playerTileNodes.push_back(tileNode);
        linkCompatibleTiles(tileNode, playerTile->tile);
    }
}

const std::vector<ITileNode*>& TileGraph::getPlayerTileNodes() {
    return _playerTileNodes;
}
