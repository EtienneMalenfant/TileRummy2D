#include <gui/main_zone_components/meld_containers_factory.h>

using namespace gui;

MeldContainersFactory::MeldContainersFactory(const std::list<const IMeld*>* gameMelds, std::vector<ITileContainer*>* meldsById, ITileFactory* tileFactory, IPlayerController* playerController) 
    : _gameMelds(gameMelds), _meldsById(meldsById), _tileFactory(tileFactory), _playerController(playerController)
{
    _gameHasMeld.reserve(meldsById->capacity());
}

void MeldContainersFactory::resetBoolArray() {
    _gameHasMeld.resize(_meldsById->size());
    for (int i{}; i < _gameHasMeld.size(); i++) {
        _gameHasMeld[i] = false;
    }
}

void MeldContainersFactory::createNewMeldContainers() {
    // je passe sur tous les melds du jeu pour créer les conteneurs si nécessaire
    for (const IMeld* meld : *_gameMelds) {
        // si le meld n'est pas déjà dans la liste
        if (meld->getId() >= _meldsById->size()) {
            _meldsById->resize(meld->getId() + 1);
        }
        // si le meld n'est pas déjà dans la liste
        if (meld->getId() == _meldsById->size() || _meldsById->at(meld->getId()) == nullptr) {
            _meldsById->at(meld->getId()) = new MeldContainer(meld, _tileFactory, _playerController);
        }
    }
}

void MeldContainersFactory::deleteOldMeldContainers(std::vector<int>** meldsIdMap, int meldMapSize) {
    resetBoolArray();
    // je passe sur tous les melds du jeu pour détecter ceux qui sont présents
    for (const IMeld* meld : *_gameMelds) {
        _gameHasMeld[meld->getId()] = true;
    }
    
    // supprimer les conteneurs inutiles (plus dans le jeu)
    // dans la map de melds
    for (int row = 0; row < meldMapSize; row++) {
        std::vector<int>* meldsRow = meldsIdMap[row];
        for (auto it = meldsRow->begin(); it != meldsRow->end(); it++) {
            int meldId = *it;
            if (_gameHasMeld[meldId] == false) {
                meldsRow->erase(it);
                it--;
            }
        }
    }

    // dans les melds par id
    for (int i{}; i < _gameHasMeld.size(); i++) {
        if (_gameHasMeld[i] == false) {
            delete _meldsById->at(i);
            _meldsById->at(i) = nullptr;
        }
    }
}