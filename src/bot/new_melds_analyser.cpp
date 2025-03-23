#include <bot/actions_analyser.h>

using namespace bot;

void NewMeldsAnalyser::getNewSets(std::list<std::list<Action*>*>* newMelds) {
    updateTilesArray();
    // on passe sur chaque valeur
    for (int valueIndex = 0; valueIndex < 13; valueIndex++) {
        bool hasColor[4] {false};
        // on vérifie combien de couleurs sont présentes
        for (int colorIndex = 0; colorIndex < 4; colorIndex++) {
            if (_tiles[colorIndex][valueIndex] != nullptr) {
                hasColor[colorIndex] = true;
            }
        }

        int colorCount = 0;
        for (int i = 0; i < 4; i++) {
            if (hasColor[i]) {
                colorCount++;
            }
        }
        // si on au moins 3 couleurs différentes son ajoute actions pour créer le set
        if (colorCount >= 3 - _jokerCount) {
            int lastTileId = -1;
            std::list<Action*>* actions = new std::list<Action*>();
            for (int i = 0; i < 4; i++) {
                const Tile* currentTile = _tiles[i][valueIndex];
                // utiliser un joker si on en a
                if (currentTile == nullptr && _jokerCount > 0 && colorCount < 3) {
                    currentTile = _joker[_jokerCount - 1];
                    _joker[_jokerCount - 1] = nullptr;
                    _jokerCount--;
                }
                // ajouter la tuile à la liste d'actions
                if (currentTile != nullptr) {
                    actions->push_back(new Action {currentTile->uid, lastTileId, -1});
                    lastTileId = currentTile->uid;
                    _tiles[i][valueIndex] = nullptr;
                }
            }
            newMelds->push_back(actions);
        }
    }
}

void NewMeldsAnalyser::getNewSequences(std::list<std::list<Action*>*>* newMelds) {
    updateTilesArray();
    // on passe sur chaque couleur
    for (int colorIndex = 0; colorIndex < 4; colorIndex++) {
        int sequenceLength = 0;
        // on passe sur chaque valeur
        for (int valueIndex = 0; valueIndex < 13; valueIndex++) {
            if (_tiles[colorIndex][valueIndex] != nullptr) {
                sequenceLength++;
            }
            else {
                // on a une séquence d'au moins 3 tuiles
                if (sequenceLength >= 3 - _jokerCount) {
                    std::list<Action*>* actions = new std::list<Action*>();
                    const Tile* currentTile = nullptr;
                    int lastTileId {-1};
                    // créer la liste d'action avec les tuiles
                    for (int i = valueIndex - sequenceLength; i < valueIndex; i++) {
                        currentTile = _tiles[colorIndex][i];
                        actions->push_back(new Action {currentTile->uid, lastTileId, -1});
                        lastTileId = currentTile->uid;
                        _tiles[colorIndex][i] = nullptr;
                    }
                    // terminer la séquence avec des jokers
                    for (int i = sequenceLength; i < 3; i++) {
                        const Tile* joker = _joker[_jokerCount - 1];
                        // si la dernière tuile est 13, le joker est ajouté au début
                        if (currentTile->value == 13) {
                            lastTileId = actions->front()->srcTileUID;
                            actions->push_front(new Action {joker->uid, -1, lastTileId});
                        }
                        else {
                            actions->push_back(new Action {joker->uid, lastTileId, -1});
                        }
                        lastTileId = joker->uid;
                        _joker[_jokerCount - 1] = nullptr;
                        _jokerCount--;
                    }

                    newMelds->push_back(actions);
                    sequenceLength = 0;
                }
                else {
                    sequenceLength = 0;
                }
            }
        }
    }
}

// ------------------------------

std::list<std::list<Action*>*>* NewMeldsAnalyser::getActionsSequences(const std::list<const Tile*>* playerTiles, bool useJoker) {
    resetPlayerTiles();
    fillStandbyTiles(playerTiles);
    updateTilesArray();
    if (useJoker == false) {
        emptyJokers();
    }
    std::list<std::list<Action*>*>* newMelds = new std::list<std::list<Action*>* > {};
    getNewSets(newMelds);
    getNewSequences(newMelds);

    return newMelds;
}

