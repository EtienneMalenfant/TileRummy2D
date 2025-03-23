#include <game/event_listener.h>

class ConsoleGameEventListener : public IGameEventListener {
public:
    void update(ptr<IEvent> event) override;
};