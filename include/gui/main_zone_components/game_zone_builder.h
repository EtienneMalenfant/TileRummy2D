#pragma once
#include <gui/components/i_component_builder.h>
#include <gui/components/container.h>
#include <gui/game_event_handler.h>
#include <gui/events/i_window_events_publisher.h>
#include <game/game_dependencies.h>
#include <gui/main_zone_components/tile_factory.h>

namespace gui {
    class GameZoneContainerBuilder : public IComponentBuilder {
    private:
        Container* _gameZoneContainer;
        Container* _meldsContainer;
        Container* _playerTilesContainer;
        const float _mainZoneHeightRatio;
        const sf::Font* _font;
        ITileFactory* _tileFactory;

    public:
        GameZoneContainerBuilder(GameDependencies* dependencies, IGameEventHandler* eventHandler,
            IWindowEventsPublisher* eventsPublisher, float mainZoneHeightRatio);
        ~GameZoneContainerBuilder() override;
        void setEmplacement(const sf::FloatRect& emplacement) override;
        sf::Drawable* getDrawable() override;
    };

}
