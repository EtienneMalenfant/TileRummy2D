#pragma once
#include <game/player.h>
#include <game/event_listener.h>
#include <gui/components/i_updateable.h>
#include <mutex>
#include <condition_variable>

namespace gui {

    class PlayerControlsProxy : public IGameEventListener, public IPlayerController {
    private:
        bool _isUserTurn = false;
        std::mutex _mutex;
        std::condition_variable _cv;

        IGameEventListener* _eventListener;
        IPlayerController* _controller;

        ILogger* _gameLogger;
        IUpdateable* _rootUpdateable;
        std::string _userName;
        void displayNotYourTurnMessage();

    public:
        PlayerControlsProxy(IGameEventListener* eventListener, IPlayerController* controller, const std::string& userName, IUpdateable* rootUpdateable);
        ~PlayerControlsProxy() override;

        void setGameLogger(ILogger* logger);
        // IGameEventListener
        void update(ptr<IEvent> event) override;
        // IPlayerController
        void draw() override;
        bool addAction(Action* action) override;
        bool commitActions() override;
        void cancelActions() override;
        bool isStarted() const override;
    };
}

