#include <gui/info_zone_components/info_zone_builder.h>
#include <gui/info_zone_components/game_info_container.h>
#include <gui/info_zone_components/text_logger_component.h>
#include <gui/info_zone_components/players_info_component.h>
#include <gui/utilities/font_loader.h>

using namespace gui;

InfoZoneBuilder::InfoZoneBuilder(const std::vector<IPlayer*>* const players, IGameEventHandler* const gameEventHandler)
{
    sf::Font* font = FontLoader::loadFont("assets/fonts/Inconsolata-Regular.ttf");

    // Création du logger
    TextLoggerComponent* textLoggerComponent = new TextLoggerComponent(font);
    gameEventHandler->addLogger(textLoggerComponent); // il implémente ILogger
    Container* gameLogsComponent = textLoggerComponent;

    // Création des infos des joueurs
    PlayersInfoComponent* playerInfosComponent = new PlayersInfoComponent(players, font);
    Container* infosContainer = playerInfosComponent;
    gameEventHandler->addUpdeatable(playerInfosComponent); // il implémente IUpdateable

    // Création du container des infos du jeu
    _infosContainer = new GameInfosContainer(gameLogsComponent, infosContainer);
    _infosContainer->setFillColor(sf::Color(20, 75, 20));
}

InfoZoneBuilder::~InfoZoneBuilder() {
    delete _infosContainer;
}

void InfoZoneBuilder::setEmplacement(const sf::FloatRect& emplacement) {
    _infosContainer->setSize(sf::Vector2f(emplacement.width, emplacement.height));
    _infosContainer->setPosition(emplacement.left, emplacement.top);
}

sf::Drawable* InfoZoneBuilder::getDrawable() {
    return _infosContainer;
}
