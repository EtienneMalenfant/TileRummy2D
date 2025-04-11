#include <gui/events/i_window_events_publisher.h>

using namespace gui;

void WindowEventPublisher::notify(const sf::Event& event) {
    for (ptr<IWindowEventHandler> listener : _listeners[event.type]) {
        listener->handle(event);
    }
}

void WindowEventPublisher::subscribe(sf::Event::EventType type, ptr<IWindowEventHandler> listener) {
    _listeners[type].push_back(listener);
}
