#include <bot/bot_player.h>
#include <stdexcept>
#include <thread>
#include <chrono>

using namespace bot;

BotPlayer::BotPlayer(IPlayerController* controller, IPlayer* player, IActionsAnalyser* insertionsAnalyser)
    : _controller(controller), _player(player), _insertionsAnalyser(insertionsAnalyser)
{
    _newMeldsAnalyser = new NewMeldsAnalyser();
}

BotPlayer::~BotPlayer() {
    // We don't delete _controller and _player because they will be deleted by the game builder
    deleteAndClearNewMelds();
}

// ------------------------------

void BotPlayer::simulateProcessing() {
    // une range de temps aléatoire pour pas que le temps d'attente soit toujours le même
    std::uniform_int_distribution<> distribution(_baseWaitTime, _baseWaitTime + (_baseWaitTime / 2));
    int waitTime = 0;
    if (_baseWaitTime > 0) {
        waitTime = distribution(_randGen);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
}

void BotPlayer::deleteAndClearNewMelds() {
    if (_newMelds != nullptr) {
        for (std::list<Action*>* actionsList : *_newMelds) {
            for (Action* action : *actionsList) {
                delete action;
            }
            delete actionsList;
        }
        delete _newMelds;
        _newMelds = nullptr;
    }
}

std::list<std::list<Action*>*>* BotPlayer::getNewMelds() {
    deleteAndClearNewMelds();
    return _newMeldsAnalyser->getActionsSequences(_player->getTiles(), _useJoker);
}

void BotPlayer::handleFirstTurn() {
    bool hasPlayed = false;
    if (_newMelds->size() > 0) {
        // essayer de jouer tous les melds possible pour avoir les points
        hasPlayed = playAllNewMelds();
    }
    if (hasPlayed == false) {
        _controller->draw();
    }
}

void BotPlayer::playActionList(std::list<Action*>* actionList) {
    for (Action* action : *actionList) {
        _controller->addAction(action);
    }
    delete actionList;
}

bool BotPlayer::playAllNewMelds() {
    for (std::list<Action*>* newMeldPlacement : *_newMelds) {
        playActionList(newMeldPlacement);
        newMeldPlacement = nullptr;
    }
    if (_controller->commitActions()) {
        delete _newMelds;
        _newMelds = nullptr;
        return true;
    }
    else {
        _controller->cancelActions();
    }
    return false;
}

std::list<const Tile*>* BotPlayer::filterStandbyTiles() {
    // mettre de coter les tuiles des nouveaux melds
    std::list<const Tile*>* filteredTiles = new std::list<const Tile*>();
    for (const Tile* t : *_player->getTiles()) {
        filteredTiles->push_back(t);
    }
    for (std::list<Action *>* actionsList : *_newMelds) {
        for (Action* action : *actionsList) {
            for (const Tile* tile : *filteredTiles) {
                if (tile->uid == action->srcTileUID) {
                    filteredTiles->remove(tile);
                    break;
                }
            }
        }
    }
    return filteredTiles;
}

bool BotPlayer::playInsertions() {
    std::list<const Tile*>* availableTiles = filterStandbyTiles();
    std::list<std::list<Action*>*>* insertionsList =  _insertionsAnalyser->getActionsSequences(availableTiles, _useJoker);

    bool hasPlayed = false;
    for (auto insertion : *insertionsList) {

        for (Action* action : *insertion) {
            bool hasAddedAction = _controller->addAction(action);
            if (hasAddedAction == false) {
                _controller->cancelActions();
                #ifdef DEBUG
                    throw std::runtime_error("Un bot a essayé de jouer une action impossible");
                #endif
            }
            hasPlayed = true;
        }
        delete insertion;
        insertion = nullptr;
    }
    delete insertionsList;
    delete availableTiles;
    return hasPlayed;
}

bool BotPlayer::playSomething() {
    // jouer des insertions si possible
    bool hasPlayed = playInsertions();
    // sinon jouer un nouveau meld si possible
    if (hasPlayed == false) {
        if (_newMelds->size() > 0) {
            playActionList(_newMelds->front());
            _newMelds->pop_front();
            true;
        }
    }
    return hasPlayed;
}

void BotPlayer::update(ptr<IEvent> event) {
    if (event->getType() == EventType::GIVE_TURN && _player->getName() == event->toString()) {
        simulateProcessing();
        if (_player->getTiles()->size() < 5) {
            _useJoker = true;
        }
        // calculer les nouveaux melds avant les insertions pour ne pas utiliser ces tuiles
        _newMelds = getNewMelds();
        bool newMeldsAvailable = _newMelds->size() > 0;

        // traitement spécial pour le premier tour
        if (_controller->isStarted() == false) {
            handleFirstTurn();
            return;
        }

        // essayer de jouer quelque chose
        bool hasPlayed = playSomething();
        if (hasPlayed) {
            bool commitIsValid = _controller->commitActions();
            if (commitIsValid == false) {
                // ne devrait pas arriver, c pour m'avertir
                _controller->cancelActions();
                #ifdef DEBUG
                    throw std::runtime_error("Les actions d'un joueur bot ont ete refusees");
                #endif
            }
        }
        // sinon on pige
        else {
            _controller->draw();
        }
    }
}

void BotPlayer::setWaitTime(unsigned int time) {
    _baseWaitTime = time;
}
