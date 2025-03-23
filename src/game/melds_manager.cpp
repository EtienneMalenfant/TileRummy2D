#include <game/melds_manager.h>
#include <game/melds_command.h>
#include <stdexcept>

MeldsManager::~MeldsManager() {
    clearActions();
    for (IMeld* meld : _melds) {
        delete meld;
    }
}

// ------------------------------
// méthodes privées

bool MeldsManager::validateAction(Action* action) {
    if (action->srcTileUID == -1) {
        throw std::runtime_error("MeldsManager::validateAction: l'UID de la tuile source est invalide");
        return false;
    }
    return true;
}

IMeld* MeldsManager::getDestMeld(Action* action) {
    IMeld* destMeld {};
    // tuile dans un nouveau meld
    if (action->destLeftTileUID == -1 && action->destRightTileUID == -1) {
        CreateMeldCommand* createAction = new CreateMeldCommand(&_melds);
        createAction->execute();
        destMeld = createAction->getMeld();
        _executedActions.push(createAction);
    }
    // tuiles dans un meld existant
    else {
        int neighbourTileId = action->destLeftTileUID != -1 ? action->destLeftTileUID : action->destRightTileUID;
        destMeld = findMeld(neighbourTileId);
    }
    return destMeld;
}

IMeld* MeldsManager::findMeld(int tileUID) {
    for (IMeld* meld : _melds) {
        int index = meld->findByUID(tileUID);
        if (index != -1) {
            return meld;
        }
    }
    throw std::runtime_error("MeldsManager::getDestMeld: le Meld de destination n'a pas été trouvé");
    return nullptr;
}

void MeldsManager::clearActions() {
    while (!_executedActions.empty()) {
        ICommand* command = _executedActions.top();
        _executedActions.pop();
        delete command;
    }
}

void MeldsManager::cleanUpMeld(IMeld* meld) {
    // Supprimer le meld s'il est vide
    if (meld->size() == 0) {
        ICommand* deleteAction = new DeleteMeldCommand(meld, &_melds);
        deleteAction->execute();
        _executedActions.push(deleteAction);
    }
    // Spliter le meld s'il a un trou
    else {
        for (int i{0}; i < meld->size(); i++) {
            if (meld->get(i) == nullptr) {
               splitMeld(meld, i);
            }
        }
    }
}

void MeldsManager::splitMeld(IMeld* meld, int index) {
    IMeld* newMeld {};
    CreateMeldCommand* newMeldCmd = new CreateMeldCommand(&_melds);
    newMeldCmd->execute();
    newMeld = newMeldCmd->getMeld();
    _executedActions.push(newMeldCmd);

    int lastTileId = -1;
    for (int i{index + 1}; i < meld->size(); i++) {
        Tile* currentTile = meld->get(i);
        ICommand* moveCmd = new MoveTileCommand(meld, newMeld, new Action {currentTile->uid, lastTileId, -1});
        moveCmd->execute();
        _executedActions.push(moveCmd);
        lastTileId = currentTile->uid;
    }
}

// ------------------------------

bool MeldsManager::confirm() {
    for (IMeld* meld : _melds) {
        if (!meld->isValid()) {
            return false;
        }
    }
    clearActions();
    return true;
}

void MeldsManager::cancel() {
    while (!_executedActions.empty()) {
        ICommand* command = _executedActions.top();
        _executedActions.pop();
        command->undo();
        delete command;
    }
}

void MeldsManager::moveTile(Action* action) {
    validateAction(action);
    IMeld* destMeld {getDestMeld(action)};
    IMeld* srcMeld {findMeld(action->srcTileUID)};
    // SmartMoveTile se charge de fermer les trou si c'est pertinent
    ICommand* command {new SmartMoveTileCommand(srcMeld, destMeld, action)};
    command->execute();
    _executedActions.push(command);
    cleanUpMeld(srcMeld);
}

void MeldsManager::putTile(Action* action, Tile* tile) {
    validateAction(action);
    IMeld* destMeld {getDestMeld(action)};
    ICommand* command {new PutTileCommand(tile, destMeld, action)};
    command->execute();
    _executedActions.push(command);
}


const std::list<const IMeld*>* MeldsManager::getMelds() const {
    return reinterpret_cast<const std::list<const IMeld*>*>(&_melds);
}
