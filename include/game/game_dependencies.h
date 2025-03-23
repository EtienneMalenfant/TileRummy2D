#pragma once
#include <game/melds_manager.h>
#include <game/player.h>
#include <game/tile_stock.h>
#include <game/rummy_game.h>

class GameDependencies {
public:
    IGameEventPublisher* const eventPublisher;
    const IMeldsContainer* const meldsContainer;
    IPlayerController* const playerController;
    const IPlayer* const currentPlayer;
    const std::vector<IPlayer*>* players;

    GameDependencies(IGameEventPublisher* eventPublisher, const IMeldsContainer* meldsContainer, 
        IPlayerController* playerController, const IPlayer* currentPlayer, const std::vector<IPlayer*>* players) :
        eventPublisher(eventPublisher), meldsContainer(meldsContainer), 
        playerController(playerController), currentPlayer(currentPlayer), players(players) {}
    ~GameDependencies() = default;
};