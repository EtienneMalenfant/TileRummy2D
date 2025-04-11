#pragma once
#include <SFML/Graphics.hpp>
#include <gui/events/i_window_event_handler.h>
#include <smart_ptr.h>

namespace gui {

    class IWindowEventsPublisher {
    public:
        virtual void notify(const sf::Event& event) = 0;
        virtual void subscribe(sf::Event::EventType type, ptr<IWindowEventHandler> listener) = 0;

        virtual ~IWindowEventsPublisher() = default;
    };

    class WindowEventPublisher : public IWindowEventsPublisher {
    private :
        std::vector<ptr<IWindowEventHandler>> _listeners [sf::Event::Count];
    public:
        WindowEventPublisher() = default;
        void notify(const sf::Event& event) override;
        void subscribe(sf::Event::EventType type, ptr<IWindowEventHandler> listener) override;
    };
}
