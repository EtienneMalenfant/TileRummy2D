#pragma once
#include <gui/events/i_window_event_handler.h>
#include <gui/settings.h>

namespace gui {
    class WindowResizeEventsHandler : public IWindowEventHandler {
    private:
        sf::RenderWindow* _window;
        IWindowSettings* _settings;
    public:
        WindowResizeEventsHandler(sf::RenderWindow* window, IWindowSettings* settings);
        ~WindowResizeEventsHandler() override = default;
        void handle(const sf::Event& event) override;
    };
    
}