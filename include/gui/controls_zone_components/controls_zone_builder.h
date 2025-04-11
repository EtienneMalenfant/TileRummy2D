#pragma once
#include <gui/components/i_component_builder.h>
#include <gui/events/i_window_events_publisher.h>
#include <game/player.h>
#include <gui/components/container.h>

namespace gui {
    class GameControlsBuilder : public IComponentBuilder {
    private:
        Container* _controlsContainer;
        gui::IWindowEventHandler* _handler;
    public:
        GameControlsBuilder(IPlayerController* const controller, IWindowEventsPublisher* const eventPublisher);
        // pas de delete, le container va se faire delete par le IWindowEventsPublisher
        void setEmplacement(const sf::FloatRect& emplacement) override;
        sf::Drawable* getDrawable() override;

    };
}
