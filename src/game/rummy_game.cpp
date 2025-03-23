#include <game/rummy_game.h>

RummyGame::RummyGame(std::vector<IPlayer*>* players, IGameEventPublisher* publisher) 
    : _players(players), _eventPublisher(publisher) {
        _nbPlayers = players->size();
    }

RummyGame::~RummyGame() {
    for (int i = 0; i < _nbPlayers; i++) {
        delete (*_players)[i];
    }
    delete[] _players;
}

bool RummyGame::isOver() {
    for (int i = 0; i < _nbPlayers; i++) {
        if ((*_players)[i]->getTiles()->size() == 0) {
            return true;
        }
    }
    return false;
}

// ------------------------------

void RummyGame::start() {
    _eventPublisher->notifyListeners(std::make_shared<GiveTurnEvent>((*_players)[_currentPlayerIndex]->getName()));
}

void RummyGame::update( ptr<IEvent> event) {
    if (event->getType() == EventType::PLAYER_ACTION) {
        if (isOver()) {
            _eventPublisher->notifyListeners(std::make_shared<GameOverEvent>((*_players)[_currentPlayerIndex]->getName()));
        }
        else {
            _currentPlayerIndex = (_currentPlayerIndex + 1) % _nbPlayers;
            _eventPublisher->notifyListeners(std::make_shared<GiveTurnEvent>((*_players)[_currentPlayerIndex]->getName()));
        }
    }
}