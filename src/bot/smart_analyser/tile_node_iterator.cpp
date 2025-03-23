#include <bot/smart_analyser/tile_node_iterator.h>

using namespace bot;

TileNodeIterator::TileNodeIterator(std::vector<ITileNode*>* nodes) : _nodes(nodes) {}

TileNodeIterator::~TileNodeIterator() {
    delete _nodes;
}

ITileNode* TileNodeIterator::next() {
    ITileNode* node = (*_nodes)[_index];
    _index++;
    return node;
}

ITileNode* TileNodeIterator::peek() {
    return (*_nodes)[_index];
}

bool TileNodeIterator::hasNext() {
    return _index < _nodes->size();
}

void TileNodeIterator::reset() {
    _index = 0;
}
