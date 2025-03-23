#include <iostream>
#include <algorithm>
#include <game/event_listener.h>
#include <game/event_publisher.h>


void GameEventPublisher::subscribe(IGameEventListener* subscriber) {
    _subscribers.push_back(subscriber);
}

void GameEventPublisher::unsubscribe(IGameEventListener* subscriber) {
    auto it = std::find(_subscribers.begin(), _subscribers.end(), subscriber);
    _subscribers.erase(it);
}

void GameEventPublisher::notifyListeners(ptr<IEvent> event) {
    _events.push(event);
    // Pour éviter les appels récursifs infini sans quitter le contexte précédent
    if (!_isProcessing) {
        _isProcessing = true;
        while (!_events.empty()) {
            ptr<IEvent> e = _events.front();
            _events.pop();
            
            for (auto subscriber : _subscribers) {
                subscriber->update(e);
            }
        }
        _isProcessing = false;
    }
}
