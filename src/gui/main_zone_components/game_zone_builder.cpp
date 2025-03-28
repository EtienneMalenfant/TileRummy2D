#include <gui/main_zone_components/game_zone_builder.h>
#include <gui/main_zone_components/melds_manager_container.h>
#include <gui/main_zone_components/game_zone_container.h>
#include <gui/main_zone_components/player_tiles_container.h>
#include <gui/utilities/font_loader.h>
#include <gui/main_zone_components/tile_mover.h>

using namespace gui;

GameZoneContainerBuilder::GameZoneContainerBuilder(GameDependencies* dependencies, IGameEventHandler* eventHandler, 
    IWindowEventsPublisher* eventsPublisher,  float mainZoneHeightRatio)
    : _mainZoneHeightRatio(mainZoneHeightRatio) 
{
    sf::Font* font = FontLoader::loadFont("assets/fonts/Inter_28pt-Bold.ttf");
    const TileInfo* tileInfo = new TileInfo { 
        sf::Color(240, 196, 79),
        40,
        60,
        font
    };
    ITileFactory* tileFactory = new TileFactory(tileInfo);

    // zone de jeu
    MeldsManagerContainer* meldsContainer = new MeldsManagerContainer(tileFactory, dependencies->meldsContainer, dependencies->playerController);
    eventHandler->addUpdeatable(meldsContainer);
    _meldsContainer = meldsContainer;
    _meldsContainer->setFillColor(sf::Color(0, 110, 0));
    
    //zone de l'inventaire du joueur
    PlayerTilesContainer* playerTilesContainer = new PlayerTilesContainer(tileFactory, dependencies->currentPlayer);
    eventHandler->addUpdeatable(playerTilesContainer);
    _playerTilesContainer = playerTilesContainer;
    _playerTilesContainer->setFillColor(sf::Color(0, 140, 20));

    // le gestionnaire de déplacement
    TileMover* tileMover = new TileMover(meldsContainer, playerTilesContainer);
    eventsPublisher->subscribe(sf::Event::MouseButtonPressed, tileMover);
    eventsPublisher->subscribe(sf::Event::MouseButtonReleased, tileMover);
    eventsPublisher->subscribe(sf::Event::MouseMoved, tileMover);

    // les deux ensembles
    _gameZoneContainer = new GameZoneContainer(_playerTilesContainer, _meldsContainer, tileMover, _mainZoneHeightRatio);
}

GameZoneContainerBuilder::~GameZoneContainerBuilder() {
    delete _gameZoneContainer;
    // TODO delete _handler;
}

void GameZoneContainerBuilder::setEmplacement(const sf::FloatRect& emplacement) {
    _gameZoneContainer->setSize({emplacement.width, emplacement.height});
    _gameZoneContainer->setPosition(emplacement.left, emplacement.top);
}

sf::Drawable* GameZoneContainerBuilder::getDrawable() {
    return _gameZoneContainer;
}
