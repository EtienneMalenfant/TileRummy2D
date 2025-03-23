#pragma once
#include <SFML/Graphics.hpp>
#include <gui/events/i_window_event_handler.h>

namespace gui {

    class IWindowEventsPublisher {
    public:
        virtual void notify(const sf::Event& event) = 0;
        virtual void subscribe(sf::Event::EventType type, IWindowEventHandler* listener) = 0;

        virtual ~IWindowEventsPublisher() = default;
    };

    class WindowEventPublisher : public IWindowEventsPublisher {
    private :
        std::vector<IWindowEventHandler*> listeners [sf::Event::Count];
    public:
        WindowEventPublisher() = default;
        ~WindowEventPublisher() override = default;
        void notify(const sf::Event& event) override;
        void subscribe(sf::Event::EventType type, IWindowEventHandler* listener) override;
    };
}