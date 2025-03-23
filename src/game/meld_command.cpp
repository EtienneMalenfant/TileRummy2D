#include <game/melds_command.h>
#include <stdexcept>

CreateMeldCommand::CreateMeldCommand(std::list<IMeld*>* melds) : _melds(melds) {}

int CreateMeldCommand::_nextMeldUid = 0;

void CreateMeldCommand::execute() {
    _newMeld = new Meld(_nextMeldUid);
    _melds->push_back(_newMeld);
    _nextMeldUid++;
}

void CreateMeldCommand::undo() {
    _melds->remove(_newMeld);
    delete _newMeld;
}

IMeld* CreateMeldCommand::getMeld() {
    return _newMeld;
}

// ------------------------------

DeleteMeldCommand::DeleteMeldCommand(IMeld* meld, std::list<IMeld*>* melds) : _meld(meld), _melds(melds) {}

DeleteMeldCommand::~DeleteMeldCommand() {
    if (_hasUndo == false) {
        delete _meld;
    }
}

void DeleteMeldCommand::execute() {
    _melds->remove(_meld);
    // je ne delete pas le meld ici pour garder la possibilité de reconstruire le meld avec les autres commandes
}

void DeleteMeldCommand::undo() {
    _melds->push_back(_meld);
    _hasUndo = true;
}

// ------------------------------

MoveTileCommand::MoveTileCommand(IMeld* srcMeld, IMeld* destMeld, Action* action) : _srcMeld(srcMeld), _destMeld(destMeld), action(action) {}

MoveTileCommand::~MoveTileCommand() {
    delete action;
}

int MoveTileCommand::getDestIndex() {
    if (action->destLeftTileUID != -1) {
        return _destMeld->findByUID(action->destLeftTileUID) + 1;
    } else if (action->destRightTileUID != -1) {
        return _destMeld->findByUID(action->destRightTileUID);
    }
    return 0;
}

void MoveTileCommand::execute() {
    _srcIndex = _srcMeld->findByUID(action->srcTileUID);
    _tile = _srcMeld->remove(_srcIndex);

    if (_tile == nullptr) {
        throw new std::runtime_error("Erreur : tuile à déplacer introuvable");
        return;
    }

    _destIndex = getDestIndex();
    _destMeld->insert(_destIndex, _tile);
}

void MoveTileCommand::undo() {
    _srcMeld->set(_srcIndex, _tile);
    _destMeld->removeShift(_destIndex);
}

// ------------------------------

SmartMoveTileCommand::SmartMoveTileCommand(IMeld* srcMeld, IMeld* destMeld, Action* action) : MoveTileCommand(srcMeld, destMeld, action) {}

void SmartMoveTileCommand::execute() {
    MeldType srcType = _srcMeld->getType();
    _srcIndex = _srcMeld->findByUID(action->srcTileUID);

    if (srcType == MeldType::SEQUENCE && _srcIndex != 0) {
        // pour laisser un trou selement si c'éait un séquence et qu'on retire au milieu
        _tile = _srcMeld->remove(_srcIndex);
    }
    else {
        // on ferme le trou causé par la tuile enlevée
        _tile = _srcMeld->removeShift(_srcIndex);
        _hasShiftedTiles = true;
    }

    if (_tile == nullptr) {
        throw new std::runtime_error("Erreur : tuile à déplacer introuvable");
        return;
    }

    _destIndex = getDestIndex();
    _destMeld->insert(_destIndex, _tile);
}

void SmartMoveTileCommand::undo() {
    if (_destMeld == _srcMeld) {
        Tile* temp = _srcMeld->get(_srcIndex);
        _srcMeld->set(_srcIndex, _tile);
        _destMeld->set(_destIndex, temp);
        return;
    }
    if (_hasShiftedTiles) {
        _srcMeld->insert(_srcIndex, _tile);
    }
    else {
        _srcMeld->set(_srcIndex, _tile);
    }
    _destMeld->removeShift(_destIndex);
}

// ------------------------------

PutTileCommand::PutTileCommand(Tile* tile, IMeld* meld, Action* action) : _tile(tile), _meld(meld), action(action) {}

PutTileCommand::~PutTileCommand() {
    delete action;
}

void PutTileCommand::execute() {
    if (action->destLeftTileUID != -1) {
        _index = _meld->findByUID(action->destLeftTileUID) + 1;
    } else if (action->destRightTileUID != -1) {
        _index = _meld->findByUID(action->destRightTileUID);
    }
    _meld->insert(_index, _tile);
}

void PutTileCommand::undo() {
    _meld->removeShift(_index);
}
