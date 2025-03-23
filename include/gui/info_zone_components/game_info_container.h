#pragma once
#include <SFML/Graphics.hpp>
#include <game/game_dependencies.h>
#include <gui/components/container.h>

namespace gui {

    class GameInfosContainer : public Container {
    private:
        Container* _gameLogsComponent;
        Container* _playerInfosComponent;
        constexpr static float LOG_SECTION_HEIGHR_RATIO = 0.8f;
    public:
        GameInfosContainer(Container* gameLogsComponent, Container* playerInfosComponent);
        ~GameInfosContainer() override;

        void setPosition(float x, float y) override;
        void setPosition(const sf::Vector2f& position) override;
        void setSize(const sf::Vector2f& size) override;

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };
};