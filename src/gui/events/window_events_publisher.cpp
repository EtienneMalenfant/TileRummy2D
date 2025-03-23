#include <gui/events/i_window_events_publisher.h>

using namespace gui;

void WindowEventPublisher::notify(const sf::Event& event) {
    for (IWindowEventHandler* listener : listeners[event.type]) {
        listener->handle(event);
    }
}

void WindowEventPublisher::subscribe(sf::Event::EventType type, IWindowEventHandler* listener) {
    listeners[type].push_back(listener);
}