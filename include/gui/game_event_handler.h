#pragma once
#include <vector>
#include <queue>
#include <mutex>
#include <game/event_listener.h>
#include <logger.h>
#include <gui/components/i_updateable.h>

namespace gui {

    class IGameEventHandler {
    public:
        virtual void addLogger(ILogger* logger) = 0;
        virtual ILogger* getLogger() = 0;
        virtual void addUpdeatable(IUpdateable* updateable) = 0;
        virtual void handleEvents() = 0;
    };

    class GameEventHandler : public IGameEventListener, public IGameEventHandler, public IUpdateable {
    private:
        std::queue<ptr<IEvent>> _events;
        ILogger* _logger;
        std::vector<IUpdateable*> _updateables;

    public:
        GameEventHandler() = default;
        ~GameEventHandler() = default;
        void addLogger(ILogger* logger) override;
        ILogger* getLogger() override;
        void addUpdeatable(IUpdateable* updateable) override;
        void handleEvents() override; // se fait appeler par le GUI

        // IGameEventListener
        void update(ptr<IEvent> event) override; // se fait appeler par le publisher du jeu

        // IUpdateable
        void update() override;
    };
}