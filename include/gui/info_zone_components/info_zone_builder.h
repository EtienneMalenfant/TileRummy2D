#pragma once
#include <SFML/Graphics.hpp>
#include <gui/components/i_component_builder.h>
#include <gui/game_event_handler.h>
#include <gui/components/container.h>
#include <logger.h>
#include <game/player.h>
#include <vector>

namespace gui {

    class InfoZoneBuilder : public IComponentBuilder {
    private:
        Container* _infosContainer;
        sf::Font* _font;
    public:
        InfoZoneBuilder(const std::vector<IPlayer*>* const players, IGameEventHandler* const gameEventHandler);
        ~InfoZoneBuilder() override;
        void setEmplacement(const sf::FloatRect& emplacement) override;
        sf::Drawable* getDrawable() override;
    };
}
