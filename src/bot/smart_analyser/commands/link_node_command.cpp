#include <bot/smart_analyser/commands/link_node_command.h>
#include <stdexcept>


using namespace bot;

LinkNodeCommand::LinkNodeCommand(ITileNode* insertedNode, std::vector<ITileNode*>* meld, ValueDirection direction) {
    _insertedNode = insertedNode;
    _meld = meld;
    if (direction == ValueDirection::Lower) {
        _direction = MeldDirection::Previous;
    }
    else {
        _direction = MeldDirection::Next;
    }
}

void LinkNodeCommand::execute() {
    _originalLeftNode = _insertedNode->unlinkMeldTile(MeldDirection::Previous);
    _orignialRightNode = _insertedNode->unlinkMeldTile(MeldDirection::Next);
    // si on retire une tuile d'un set, on le ratache
    if (_originalLeftNode != nullptr && _orignialRightNode != nullptr) {
        if (_originalLeftNode->getTileWrapper()->tile->value == _orignialRightNode->getTileWrapper()->tile->value) {
            _originalLeftNode->linkMeldTile(_orignialRightNode, MeldDirection::Next);
        }
    }
    // on insère le noeud avant le premier dans le meld
    if (_direction == MeldDirection::Previous) {
        if (_meld->size() > 0) {
            // prendre le premier noeud du meld et insérer le nouveau noeud à droite
            ITileNode* newRightNeighbour = _meld->front();
            _insertedNode->linkMeldTile(newRightNeighbour, MeldDirection::Next);
        }
        _meld->insert(_meld->begin(), _insertedNode);
    }
    // on insère le noeud après le dernier dans le meld
    else {
        if (_meld->size() > 0) {
            // prendre le dernier noeud du meld et insérer le nouveau noeud à gauche
            ITileNode* newLeftNeighbour = _meld->back();
            _insertedNode->linkMeldTile(newLeftNeighbour, MeldDirection::Previous);
        }
        _meld->push_back(_insertedNode);
    }
}

void LinkNodeCommand::undo() {
    if (_direction == MeldDirection::Previous) {
        if (_insertedNode != _meld->front()) {
            throw new std::runtime_error( "Erreur: le noeud inséré n'est pas le premier du meld");
        }
        _meld->erase(_meld->begin());
    }
    else {
        if (_insertedNode != _meld->back()) {
            throw new std::runtime_error( "Erreur: le noeud inséré n'est pas le dernier du meld");
        }
        _meld->pop_back();
    }
    _insertedNode->unlinkMeldTile(MeldDirection::Next);
    _insertedNode->unlinkMeldTile(MeldDirection::Previous);

    if (_originalLeftNode != nullptr) {
        _insertedNode->linkMeldTile(_originalLeftNode, MeldDirection::Previous);
    }
    if (_orignialRightNode != nullptr) {
        _insertedNode->linkMeldTile(_orignialRightNode, MeldDirection::Next);
    }

}
