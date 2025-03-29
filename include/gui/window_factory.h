#pragma once
#include <gui/window.h>
#include <gui/settings.h>
#include <gui/game_event_handler.h>
#include <gui/components/app_root_component.h>
#include <game/game_dependencies.h>
#include <settings/app_settings.h>

namespace gui {

    class IWindowFactory {
        public:
            virtual IWindow* createWindow() = 0;
            virtual ~IWindowFactory() = default;
    };

    class WindowFactory : public IWindowFactory {
    private:
        GameDependencies* _dependencies;
        const AppSettings* _appSettings;
        IWindowSettings* _settings;

        IComponentBuilder* _gameZoneBuilder;
        IComponentBuilder* _gameInfosBuilder;
        IComponentBuilder* _gameControlsBuilder;

        IWindowEventsPublisher* _eventPublisher;

        constexpr static float HEIGHT_SEPARATION_RATIO = 0.8;
        constexpr static float WIDTH_SEPARATION_RATIO = 0.86;

        void organizeComponentsSize();
        unsigned int getWindowStyle();
        void configWindowEventHandler(sf::RenderWindow* renderWindow);
        sf::Drawable* buildAppRoot();
    public:
        WindowFactory(GameDependencies* dependencies, IGameEventHandler* gameEventHandler, IWindowSettings* winSettings, const AppSettings* appSettings);
        ~WindowFactory() override;
        IWindow* createWindow() override;
    };
}
