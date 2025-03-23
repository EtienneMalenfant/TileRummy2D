#pragma once
#include <game/game_dependencies.h>
#include <gui/window_factory.h>
#include <gui/player_controls_proxy.h>

namespace gui {

    class IGameInterface {
    public:
        virtual IGameEventListener* getEventListener() const = 0;
        virtual void start() = 0;
        virtual bool isRunning() const = 0;
        virtual void refresh() = 0;
    };

    class WindowGameInterface : public IGameInterface {
    private:
        GameDependencies* _dependencies;
        IGameEventListener* _gameEventListener;
        IGameEventHandler* _gameEventHandler;
        IWindow* _window;
        PlayerControlsProxy* _playerControlsProxy {nullptr}; // pour set le logger
    public:
        WindowGameInterface(GameDependencies* dependencies);
        IGameEventListener* getEventListener() const override;
        void start() override;
        bool isRunning() const override;
        void refresh() override;
    };
};
