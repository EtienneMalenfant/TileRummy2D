#pragma once
#include <SFML/Graphics.hpp>
#include <gui/components/i_updateable.h>
#include <game/player.h>
#include <gui/components/container.h>

namespace gui {
    
    class PlayersInfoComponent : public Container, public IUpdateable {
        private:
            const std::vector<IPlayer*>* const _players;
            sf::Text _playersInfo[4];
            const int _nbPlayers;
            const sf::Font* _font;
            const u_int _fontSize = 20;

            void updatePosition();
        public:
            PlayersInfoComponent(const std::vector<IPlayer*>* const players, const sf::Font* font);
            ~PlayersInfoComponent() override = default;

            void setPosition(float x, float y) override;
            void setPosition(const sf::Vector2f& position) override;

            void update() override;
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        };
    
}