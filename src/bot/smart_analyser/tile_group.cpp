#include <bot/smart_analyser/tile_group.h>

using namespace bot;


InsertionGroup::InsertionGroup(ITileNode* insertedTile, ITileNode* leftNeighbour, ITileNode* rightNeighbour) :
    _insertedTile(insertedTile), _rightNeighbour(rightNeighbour), _leftNeighbour(leftNeighbour) {}


bool InsertionGroup::contains(ITileNode* tileNode) {
    return _insertedTile == tileNode || _rightNeighbour == tileNode || _leftNeighbour == tileNode;
}

int InsertionGroup::getPlayerTileCount() {
    return 1;
}

std::list<Action*>* InsertionGroup::toActions() {
    std::list<Action*>* actions = new std::list<Action*>();
    int leftDestId = -1;
    int rightDestId = -1;
    if (_leftNeighbour != nullptr) {
        leftDestId = _leftNeighbour->getTileWrapper()->tile->uid;
    }
    if (_rightNeighbour != nullptr) {
        rightDestId = _rightNeighbour->getTileWrapper()->tile->uid;
    }
    actions->push_back(new Action{_insertedTile->getTileWrapper()->tile->uid, leftDestId, rightDestId});
    return actions;
}

// ------------------------------

MeldRebuildGroup::MeldRebuildGroup(std::vector<ITileNode*> meldNodes) : _meldNodes(meldNodes) {}

bool MeldRebuildGroup::contains(ITileNode* tileNode) {
    for (ITileNode* node : _meldNodes) {
        if (node == tileNode) {
            return true;
        }
    }
    return false;
}

int MeldRebuildGroup::getPlayerTileCount() {
    int count = 0;
    for (ITileNode* node : _meldNodes) {
        if (node->getTileWrapper()->isPlayerTile) {
            count++;
        }
    }
    return count;
}

std::list<Action*>* MeldRebuildGroup::toActions() {
    std::list<Action*>* actions = new std::list<Action*>();
    int lastId = -1;
    for (ITileNode* node : _meldNodes) {
        const Tile* tile = node->getTileWrapper()->tile;
        actions->push_back(new Action{tile->uid, lastId, -1});
        lastId = tile->uid;
    }
    return actions;
}
