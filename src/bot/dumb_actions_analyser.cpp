#include <math.h>
#include <stdexcept>
#include <bot/actions_analyser.h>

using namespace bot;

DumbActionsAnalyser::DumbActionsAnalyser(IMeldsContainer* meldsContainer)
    : _meldsContainer(meldsContainer), BaseActionsAnalyser()
{}

void DumbActionsAnalyser::sortMelds() {
    for (const IMeld* meld : *_meldsContainer->getMelds()) {
        switch (meld->getType()) {
            case MeldType::SEQUENCE:
                _sequences.push_back(meld);
                break;
            case MeldType::SET:
                _sets.push_back(meld);
                break;
            case MeldType::INVALID:
                _invalids.push_back(meld);
                break;
        }
    }
}

void DumbActionsAnalyser::playOnSequences(std::list<std::list<Action*>*>* insertions) {
    for (const IMeld* sequence : _sequences) {
        // insertion au debut
        const Tile* firstTile = sequence->get(0);
        const Tile* tile = _tiles[(int)firstTile->color] [firstTile->value - 2];
        if (firstTile->value > 1 && tile != nullptr) {
            std::list<Action*>* insertion = new std::list<Action*>();
            insertion->push_back(new Action {tile->uid, -1, firstTile->uid});
            insertions->push_back(insertion);
            continue;
        }
        // insertion a la fin
        const Tile* lastTile = sequence->get(sequence->size() - 1);
        tile = _tiles[(int)lastTile->color] [lastTile->value];
        if (lastTile->value < 13 && tile != nullptr) {
            std::list<Action*>* insertion = new std::list<Action*>();
            insertion->push_back(new Action {tile->uid, lastTile->uid, -1});
            insertions->push_back(insertion);
            continue;
        }
        // insertion au milieu
        handleMiddleInsertions(insertions, sequence);
    }
}

void DumbActionsAnalyser::handleMiddleInsertions(std::list<std::list<Action*>*>* insertions, const IMeld* sequence) {
    for (int i{2}; i < sequence->size() - 2; i++) {
        const Tile* tileToInsert = _tiles[(int)sequence->get(i)->color][sequence->get(i)->value - 1];
        if (tileToInsert != nullptr) {
            // placer la tuile dans un meld vide
            std::list<Action*>* insertion = new std::list<Action*>();
            insertion->push_back(new Action {tileToInsert->uid, -1, -1});
            insertions->push_back(insertion);
            int lastId {tileToInsert->uid};
            // ajouter les tuiles à droite dans le nouveau meld de la tuile
            for (int j{i + 1}; j < sequence->size(); j++) {
                insertion->push_back(new Action {sequence->get(j)->uid, lastId, -1});
                lastId = sequence->get(j)->uid;
            }
        }
    }
}

void DumbActionsAnalyser::playOnSets(std::list<std::list<Action*>*>* insertions) {
    for (const IMeld* set : _sets) {
        // set plein
        if (set->size() == 4) {
            continue;
        }
        // verifier les couleurs du set
        bool hasColor[4] {false};
        for (int i{0}; i < set->size(); i++) {
            hasColor[(int)set->get(i)->color] = true;
        }
        // obetnir la couleur manquante du set
        int missingColor;
        for (int i{0}; i < 4; i++) {
            if (!hasColor[i]) {
                missingColor = i;
                break;
            }
        }
        // insertion si on a la tuile manquante
        if (_tiles[missingColor][set->get(0)->value - 1] != nullptr) {
            int tileId = _tiles[missingColor][set->get(0)->value - 1]->uid;
            std::list<Action*>* insertion = new std::list<Action*>();
            insertion->push_back(new Action {tileId, set->get(set->size() -1)->uid, -1});
            insertions->push_back(insertion);
        }
    }
}

void DumbActionsAnalyser::removeSameTiles(std::list<std::list<Action*>*>* insertions) {
    bool hasTile[106] { false };
    auto it = insertions->begin();
    while (it != insertions->end()) {
        // on assume qu'il y aura toujours 1 action list dans cette classe
        std::list<Action*>* actionsList = *it;
        Action* action = actionsList->front();
        if (hasTile[action->srcTileUID]) {
            it = insertions->erase(it);
        }
        else {
            hasTile[action->srcTileUID] = true;
            it++;
        }
    }
}

void DumbActionsAnalyser::reset() {
    _sequences.clear();
    _sets.clear();
    _invalids.clear();
    _jokerCount = 0;
    _joker[0] = nullptr;
    _joker[1] = nullptr;
    for (int i{0}; i < 4; i++) {
        std::fill(_tiles[i], _tiles[i] + 13, nullptr);
    }
}

// ------------------------------

std::list<std::list<Action*>*>* DumbActionsAnalyser::getActionsSequences(const std::list<const Tile*>* playerTiles, bool useJoker) {
    reset();
    sortMelds();
    fillStandbyTiles(playerTiles);
    updateTilesArray();
    if (useJoker == false) {
        emptyJokers();
    }
    auto insertions = new std::list<std::list<Action*>*>();
    playOnSequences(insertions);
    playOnSets(insertions);
    removeSameTiles(insertions);
    return insertions;
}
