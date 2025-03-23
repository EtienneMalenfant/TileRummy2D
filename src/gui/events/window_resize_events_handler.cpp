
#include <gui/events/window_resize_events_handler.h>
#include <gui/window.h>

using namespace gui;

WindowResizeEventsHandler::WindowResizeEventsHandler(sf::RenderWindow* window, IWindowSettings* settings) 
    : _window(window), _settings(settings) {}

void WindowResizeEventsHandler::handle(const sf::Event& event) {
    if (event.type == sf::Event::Closed) {
        _window->close();
    }
    else if (event.type == sf::Event::Resized) {
        float targetAspectRatio = (float)_settings->getXRes() / _settings->getYRes();
        sf::View view(sf::FloatRect(0, 0, _settings->getXRes(), _settings->getYRes()));

        sf::Vector2u windowSize = _window->getSize();
        float currentAspectRation = (float) windowSize.x / windowSize.y;

        if (currentAspectRation > targetAspectRatio) {
            float widthAdjustment = targetAspectRatio / currentAspectRation;
            view.setViewport({(1.f - widthAdjustment) / 2.f, 0.f, widthAdjustment, 1.f});
        }
        else {
            float heightAdjustment = currentAspectRation / targetAspectRatio;
            view.setViewport({0.f, (1.f - heightAdjustment) / 2.f, 1.f, heightAdjustment});
        }

        _window->setView(view);
    }
}