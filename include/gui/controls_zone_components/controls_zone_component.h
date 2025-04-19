#pragma once
#include <vector>
#include <gui/components/container.h>
#include <gui/events/i_window_event_handler.h>
#include <gui/components/button.h>
#include <game/player.h>

namespace gui {

    class ControlsZoneComponent : public Container, public IWindowEventHandler {
    private:
        static const int _nbButtons = 3;
        Button* _buttons[_nbButtons];
        sf::Font* _font;
        sf::Color _buttonColor = sf::Color(235, 174, 52);

        IPlayerController* const _controller;
        const int _buttonSpacing = 15;
    public:
        ControlsZoneComponent(IPlayerController* controller);
        ~ControlsZoneComponent() override;
        // sf::Shape
        void setPosition(const sf::Vector2f& position) override;
        void setPosition(float x, float y) override;
        void setSize(const sf::Vector2f& size) override;
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        // IWindowEventHandler
        void handle(const sf::Event& event) override;
    };
}
