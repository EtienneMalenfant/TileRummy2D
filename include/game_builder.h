#pragma once
#include <string>
#include <game/rummy_game.h>
#include <game/game_dependencies.h>
#include <bot/actions_analyser.h>
#include <bot/bot_player.h>

class IGameBuilder {
public:
    virtual void reset() = 0;
    virtual void addBot(const std::string& name) = 0;
    virtual void addPlayer(const std::string& name) = 0;
    virtual void setStockSeed(unsigned int seed) = 0;
    virtual void setBotWaitTime(unsigned int time) = 0;

    virtual IGame* getGame() = 0;
    virtual GameDependencies* getDependencies() = 0;

    virtual ~IGameBuilder() = default;
};

class RummyGameBuilder : public IGameBuilder {
private:

    unsigned int _waitTime {400};
    ILogger* _logger;
    IGameEventPublisher* _eventPublisher {};
    IMeldsContainer* _meldsContainer {};
    IMeldsManager* _meldsManager {};
    TileStock* _stock {};

    std::vector<IPlayer*>* _players {};
    IPlayerController* _guiPlayerController {};
    IPlayer* _guiPlayer {};

    bot::IBotPlayer* _botPlayer[4] {}; // pour paramétrer les bots
    bot::IActionsAnalyser* _insertionsAnalyser {};
    bot::IActionsAnalyser* _newMeldsAnalyser {};
    int _difficultyLevel {1};

    void init();
    void initActionsAnalyser();
    void deleteObjects();
    void setNullPointers();
    bool dependenciesAreSet();
    bool gameIsValid();
public:
    RummyGameBuilder();
    RummyGameBuilder(int difficultyLevel);
    ~RummyGameBuilder() override;
    void reset() override;
    void addBot(const std::string& name) override;
    void addPlayer(const std::string& name) override;
    void setStockSeed(unsigned int seed) override;
    void setBotWaitTime(unsigned int time) override;

    IGame* getGame() override;
    GameDependencies* getDependencies() override;

};
