#include <bot/smart_analyser/tile_node.h>

using namespace bot;


TileNode::TileNode(TileWrapper* tileWrapper) : _tileWrapper(tileWrapper) {
    _allNeighbours = {&_lowerValueNeighbours, &_sameValueNeighbours, &_higherValueNeighbours};
}

// ------------------------------/
// privée

void TileNode::addMeldTile(ITileNode* node) {
    int currentTileIndex  = _tileWrapper->meld->findByUID(_tileWrapper->tile->uid);
    int newNodeIndex = node->getTileWrapper()->meld->findByUID(node->getTileWrapper()->tile->uid);
    if (newNodeIndex == currentTileIndex - 1) {
        linkMeldTile(node, MeldDirection::Previous);
    }
    else if (newNodeIndex == currentTileIndex + 1) {
        linkMeldTile(node, MeldDirection::Next);
    }
}

bool TileNode::hasNeighbour(ITileNode* tileNode) {
    for (std::vector<ITileNode*>* valueNodes : _allNeighbours) {
        for (ITileNode* node : *valueNodes) {
            if (node == tileNode) {
                return true;
            }
        }
    }
    return false;
}


void TileNode::buildIterator(std::vector<ITileNode*>* meldTiles, MeldDirection d) {
    if (d == MeldDirection::Previous && _meldPrevious != nullptr) {
        _meldPrevious->buildIterator(meldTiles, d);
    }
    meldTiles->push_back(this);
    if (d == MeldDirection::Next && _meldNext != nullptr) {
        _meldNext->buildIterator(meldTiles, d);
    }
}

// ------------------------------
// public

void TileNode::addCompatibleNode(ITileNode* tileNode) {
    const Tile* addedTile = tileNode->getTileWrapper()->tile;
    int thisTileValue = _tileWrapper->tile->value;

    if (addedTile->isJoker) {
        if (thisTileValue > 1) {
            _lowerValueNeighbours.push_back(tileNode);
        }
        if (thisTileValue < 13) {
            _higherValueNeighbours.push_back(tileNode);
        }
        _sameValueNeighbours.push_back(tileNode);
    }
    else if (addedTile->value < thisTileValue) {
        _lowerValueNeighbours.push_back(tileNode);
    }
    else if (addedTile->value == thisTileValue) {
        _sameValueNeighbours.push_back(tileNode);
    }
    else if (addedTile->value > thisTileValue) {
        _higherValueNeighbours.push_back(tileNode);
    }
    // faire le lien pour une tuile du meme meld
    if (tileNode->getTileWrapper()->meld != nullptr) {
        if (tileNode->getTileWrapper()->meld == _tileWrapper->meld) {
            addMeldTile(tileNode);
        }
    }
}

void TileNode::linkMeldTile(ITileNode* node, MeldDirection meldDirection) {
    if (meldDirection == MeldDirection::Next) {
        _meldNext = (TileNode*)node;
        _meldNext->_meldPrevious = this;
    }
    else {
        _meldPrevious = (TileNode*)node;
        _meldPrevious->_meldNext = this;
    }
}

ITileNode* TileNode::unlinkMeldTile(MeldDirection meldDirection) {
    ITileNode* unlinkedNode = nullptr;
    if (meldDirection == MeldDirection::Next) {
        unlinkedNode = _meldNext;
        if (_meldNext != nullptr) {
            _meldNext->_meldPrevious = nullptr;
        }
        _meldNext = nullptr;
    }
    else {
        unlinkedNode = _meldPrevious;
        if (_meldPrevious != nullptr) {
            _meldPrevious->_meldNext = nullptr;
        }
        _meldPrevious = nullptr;
    }
    return unlinkedNode;
}

int TileNode::getMeldTileCount() {
    int count = 0;
    Iterator<ITileNode*>* it = getMeldIterator();
    while (it->hasNext()) {
        it->next();
        count++;
    }
    delete it;
    return count;
}

ITileNode* TileNode::getMeldTile(MeldDirection meldDirection) {
    if (meldDirection == MeldDirection::Next) {
        return _meldNext;
    }
    else {
        return _meldPrevious;
    }
}

int TileNode::getMeldTileCount(MeldDirection meldDirection) {
    if (meldDirection == MeldDirection::Next) {
        int nextCout = 0;
        TileNode* next = _meldNext;
        while (next != nullptr) {
            nextCout++;
            next = next->_meldNext;
        }
        return nextCout;
    }
    else {
        int previousCount = 0;
        TileNode* previous = _meldPrevious;
        while (previous != nullptr) {
            previousCount++;
            previous = previous->_meldPrevious;
        }
        return previousCount;
    }
    return -1;
}


TileWrapper* TileNode::getTileWrapper() {
    return _tileWrapper;
}

const std::vector<ITileNode*>& TileNode::getCompatibleNodes(ValueDirection direction) {
    return *_allNeighbours[(int)direction];
}

MeldType TileNode::evaluateMeldType() {
    if (getMeldTileCount() < 3) {
        return MeldType::INVALID;
    }
    Iterator<ITileNode*>* it = getMeldIterator();
    MeldType type = MeldType::INVALID;
    Color color = _tileWrapper->tile->color;
    // vérification simpliste du type de meld
    bool sameColor = true;
    int maxStepBetweenValues = 0;
    int lastValue = it->peek()->getTileWrapper()->tile->value;
    while (it->hasNext()) {
        TileWrapper* wrapper = it->next()->getTileWrapper();
        if( wrapper->tile->color != color && wrapper->tile->isJoker == false) {
            sameColor = false;
        }
        if (wrapper->tile->isJoker) {
            lastValue++;
            continue;
        }
        int step = wrapper->tile->value - lastValue;
        maxStepBetweenValues = std::max(maxStepBetweenValues, step);
        lastValue = wrapper->tile->value;
    }
    if (sameColor && maxStepBetweenValues == 1) {
        type = MeldType::SEQUENCE;
    }
    else if (sameColor == false) {
        type = MeldType::SET;
    }
    delete it;
    return type;
}

Iterator<ITileNode*>* TileNode::getMeldIterator() {
    std::vector<ITileNode*>* meldTiles = new std::vector<ITileNode*>();
    if (_meldPrevious != nullptr) {
        _meldPrevious->buildIterator(meldTiles, MeldDirection::Previous);
    }
    meldTiles->push_back(this);
    if (_meldNext != nullptr) {
        _meldNext->buildIterator(meldTiles, MeldDirection::Next);
    }
    return new TileNodeIterator(meldTiles);
}

// ------------------------------


MeldDirection EnumConversion::getMeldDirection(ValueDirection d) {
    if (d == ValueDirection::Lower) {
        return MeldDirection::Previous;
    }
    else if (d == ValueDirection::Higher) {
        return MeldDirection::Next;
    }
    return MeldDirection::Next;
}

