#pragma once
#include <game/game_dependencies.h>
#include <gui/window_factory.h>
#include <gui/player_controls_proxy.h>
#include <settings/app_settings.h>

namespace gui {

    class IGameInterface {
    public:
        virtual IGameEventListener* getEventListener() const = 0;
        virtual void start() = 0;
        virtual bool isRunning() const = 0;
        virtual void refresh() = 0;
        virtual ~IGameInterface() = default;
    };

    class WindowGameInterface : public IGameInterface {
    private:
        GameDependencies* _dependencies;
        IGameEventListener* _gameEventListener;
        IGameEventHandler* _gameEventHandler;
        IWindow* _window;
        IWindowFactory* _windowFactory;
        IWindowSettings* _windowSettings;
        PlayerControlsProxy* _playerControlsProxy {nullptr}; // pour set le logger
        const AppSettings* _appSettings;
    public:
        WindowGameInterface(GameDependencies* dependencies, const AppSettings& appSettings);
        ~WindowGameInterface() override;
        IGameEventListener* getEventListener() const override;
        void start() override;
        bool isRunning() const override;
        void refresh() override;
    };
};
