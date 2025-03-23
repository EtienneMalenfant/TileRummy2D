#include <gui/game_event_handler.h>
#include <thread>
#include <chrono>

using namespace gui;

// IGameEventListener
// on accumule les events et la fenetre les traites par handlesEvents
void GameEventHandler::update(ptr<IEvent> event) {
    _events.push(event);
    // sleep qui aide à éviter les segmentations faults si le temps d'attente des bots est de 0
    std::this_thread::sleep_for(std::chrono::milliseconds(1)); 
}

// IGameEventHandler
void GameEventHandler::handleEvents() {
    while (!_events.empty()) {
        ptr<IEvent> event = _events.front();
        _events.pop();

        if (event->getType() == EventType::PLAYER_ACTION || event->getType() == EventType::GAME_STATUS) {
            _logger->log(event->toString());
            update();
        }
    }
}

void GameEventHandler::addLogger(ILogger* logger) {
    _logger = logger;
}

ILogger* GameEventHandler::getLogger() {
    return _logger;
}

void GameEventHandler::addUpdeatable(IUpdateable* updateable) {
    _updateables.push_back(updateable);
}

void GameEventHandler::update() {
    for (IUpdateable* updateable : _updateables) {
        updateable->update();
    }
}
