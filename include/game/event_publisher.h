#pragma once
#include <string>
#include <vector>
#include <queue>

#include <game/event_listener.h>

class IGameEventPublisher {
public:
    virtual void subscribe(IGameEventListener* subscriber) = 0;
    virtual void unsubscribe(IGameEventListener* subscriber) = 0;
    virtual void notifyListeners(ptr<IEvent> event) = 0;
    virtual ~IGameEventPublisher() = default;
};

class GameEventPublisher : public IGameEventPublisher {
private:
    std::vector<IGameEventListener*> _subscribers;
    std::queue<ptr<IEvent>> _events;
    bool _isProcessing {false};
public:
    GameEventPublisher() = default;
    void subscribe(IGameEventListener* subscriber) override;
    void unsubscribe(IGameEventListener* subscriber) override;
    void notifyListeners(ptr<IEvent> event) override;
};
