#pragma once
#include <game/event.h>
#include <memory>

template <typename T>
using ptr = std::shared_ptr<T>;

class IGameEventListener {
public:
    virtual void update(ptr<IEvent> event) = 0;
};
    