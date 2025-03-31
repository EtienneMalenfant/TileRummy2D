#include <gui/events/i_window_events_publisher.h>

using namespace gui;

WindowEventPublisher::~WindowEventPublisher() {
    for (int i = 0; i < sf::Event::Count; ++i) {
        for (IWindowEventHandler* listener : _listeners[i]) {
            delete listener;
        }
    }
}

void WindowEventPublisher::notify(const sf::Event& event) {
    for (IWindowEventHandler* listener : _listeners[event.type]) {
        listener->handle(event);
    }
}

void WindowEventPublisher::subscribe(sf::Event::EventType type, IWindowEventHandler* listener) {
    _listeners[type].push_back(listener);
}
