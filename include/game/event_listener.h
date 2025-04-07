#pragma once
#include <game/event.h>
#include <smart_ptr.h>

class IGameEventListener {
public:
    virtual void update(ptr<IEvent> event) = 0;

    virtual ~IGameEventListener() = default;
};
