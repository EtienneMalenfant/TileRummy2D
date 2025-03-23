#pragma once
#include <SFML/Graphics.hpp>
#include <gui/components/i_updateable.h>
#include <gui/components/container.h>

namespace gui {

    // Le composant principale du jeu
    // il contient le composant des melds du jeu et celui des tuiles du joueur
    class GameZoneContainer : public Container {
    private:
        Container* _meldsContainer;    // toujours placé en haut
        Container* _playerTilesContainer; // toujours placé en bas
        sf::Drawable* _tileMover; // pour mettre les déplacements en avant plan
        const float _mainZoneHeightRatio;
    public:
        GameZoneContainer(Container* playerTilesContainer, Container* meldsContainer, sf::Drawable* tileMover, float mainZoneHeightRatio);
        ~GameZoneContainer() override;
        
        void setPosition(float x, float y) override;
        void setPosition(const sf::Vector2f& position) override;
        void setSize(const sf::Vector2f& size) override;
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };
}