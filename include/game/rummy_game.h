#pragma once

#include <game/player.h>
#include <game/event_listener.h>

class IGame {
public:
    virtual void start() = 0;
    virtual ~IGame() = default;
};

class RummyGame : public IGameEventListener, public IGame {
private:
    std::vector<IPlayer*>* _players {};
    IGameEventPublisher* _eventPublisher {};
    unsigned int _currentPlayerIndex {0};
    unsigned int _nbPlayers {};
    bool isOver();
public:
    RummyGame(std::vector<IPlayer*>* players, IGameEventPublisher* publisher);
    void start() override;
    void update(ptr<IEvent> event) override;
};
