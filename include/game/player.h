#pragma once
#include <list>
#include <game/action.h>
#include <game/melds_manager.h>
#include <game/tile_stock.h>
#include <game/event_publisher.h>


class IPlayer {
public:
    virtual std::string getName() const = 0;
    virtual const std::list<const Tile*>* getTiles() const = 0;

    virtual ~IPlayer() = default;
};

class IPlayerController {
public:
    virtual void draw() = 0;
    virtual bool addAction(Action* action) = 0;
    virtual bool commitActions() = 0;
    virtual void cancelActions() = 0;
    virtual bool isStarted() const = 0;

    virtual ~IPlayerController() = default;
};

class PlayerManager : public IPlayerController, public IPlayer {
private:
    std::list<Tile*> _tiles;
    std::list<Tile*> _standbyTiles;
    const std::string _name;
    ITileStock* _stock;
    IMeldsManager* _meldsManager;
    IGameEventPublisher* _eventPublisher;

    Tile* getTile(int tileId);
public:
    PlayerManager(ITileStock*, IMeldsManager*, IGameEventPublisher*, const std::string& name);
    ~PlayerManager() override;
    void draw() override;
    bool addAction(Action* action) override;
    bool commitActions() override;
    void cancelActions() override;
    bool isStarted() const override;

    const std::list<const Tile*>* getTiles() const override;
    std::string getName() const override;
};
