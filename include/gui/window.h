#pragma once
#include <SFML/Graphics.hpp>
#include <gui/events/i_window_events_publisher.h>
#include <gui/components/i_component_builder.h>

namespace gui { 

    class IWindow {
    public:
        virtual bool isOpen() const = 0;
        virtual void close() = 0;
        virtual void handleEvents() = 0;
        virtual void clear() = 0;
        virtual void draw() = 0;

        virtual ~IWindow() = default;
    };

    class Window : public IWindow {
    private:
        sf::RenderWindow* _window;
        sf::Drawable* _app;
        IWindowEventsPublisher* _eventPublisher;

        void mapCoord(sf::Event& event);
    public:
        Window(sf::RenderWindow*, sf::Drawable* rootObj, IWindowEventsPublisher* eventPublisher);
        ~Window() override;
        bool isOpen() const override;
        void close() override;
        void handleEvents() override;
        void clear() override;
        void draw() override;
    };

};