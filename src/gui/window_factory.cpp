#include <gui/window_factory.h>

#include <gui/main_zone_components/game_zone_builder.h>
#include <gui/info_zone_components/info_zone_builder.h>
#include <gui/controls_zone_components/controls_zone_builder.h>
#include <gui/components/fps_counter.h>
#include <gui/events/window_resize_events_handler.h>

using namespace gui;

WindowFactory::WindowFactory(GameDependencies* dependencies, IGameEventHandler* gameEventHandler,
    IWindowSettings* settings, const AppSettings* appSettings)
    : _settings(settings), _dependencies(dependencies), _appSettings(appSettings)
{
    _eventPublisher = new WindowEventPublisher();
    _gameZoneBuilder = new GameZoneContainerBuilder(dependencies, gameEventHandler, _eventPublisher, HEIGHT_SEPARATION_RATIO);
    _gameInfosBuilder = new InfoZoneBuilder(dependencies->players, gameEventHandler);
    _gameControlsBuilder = new GameControlsBuilder(dependencies->playerController, _eventPublisher);
}

WindowFactory::~WindowFactory() {
    delete _gameZoneBuilder;
    delete _gameInfosBuilder;
    delete _gameControlsBuilder;
    delete _eventPublisher;
}

IWindow* WindowFactory::createWindow() {
    unsigned int style = getWindowStyle();
    sf::RenderWindow* renderWindow = new sf::RenderWindow(sf::VideoMode(_settings->getBaseWindowWidth(), _settings->getBaseWindowHeight()),
         _settings->getTitle(), style);
    renderWindow->setView(sf::View(sf::FloatRect(0, 0, _settings->getBaseWindowWidth(), _settings->getBaseWindowWidth())));

    organizeComponentsSize();
    configWindowEventHandler(renderWindow);

    sf::Drawable* rootObj = buildAppRoot();
    return new Window(renderWindow, rootObj, _eventPublisher);
}

// ------------------------------

void WindowFactory::organizeComponentsSize() {
    unsigned int xSeparation = _settings->getXRes() * WIDTH_SEPARATION_RATIO;
    unsigned int ySeparation = _settings->getYRes() * HEIGHT_SEPARATION_RATIO;

    _gameZoneBuilder->setEmplacement(sf::FloatRect(0, 0, xSeparation, _settings->getYRes()));
    _gameInfosBuilder->setEmplacement(sf::FloatRect(xSeparation, 0, _settings->getXRes() - xSeparation, ySeparation));
    _gameControlsBuilder->setEmplacement(sf::FloatRect(xSeparation, ySeparation, _settings->getXRes() - xSeparation,
        _settings->getYRes() - ySeparation));
}

unsigned int WindowFactory::getWindowStyle() {
    unsigned int style = sf::Style::Default;
    if (!_settings->isResizable()) {
        style = sf::Style::Titlebar;
    }
    if (_settings->isFullscreen()) {
        style = sf::Style::Fullscreen;
    }
    return style;
}

void WindowFactory::configWindowEventHandler(sf::RenderWindow* renderWindow) {
    IWindowEventHandler* windowEventsHandler = new WindowResizeEventsHandler(renderWindow, _settings);
    _eventPublisher->subscribe(sf::Event::Closed, windowEventsHandler);
    _eventPublisher->subscribe(sf::Event::Resized, windowEventsHandler);
}

sf::Drawable* WindowFactory::buildAppRoot() {
    std::vector<sf::Drawable*>* components = new std::vector<sf::Drawable*>;
    components->push_back( _gameZoneBuilder->getDrawable());
    components->push_back( _gameInfosBuilder->getDrawable());
    components->push_back( _gameControlsBuilder->getDrawable());
    if (_appSettings->showFps) {
        components->push_back( new FpsCounter());
    }
    return new AppRootComponent(components);
}
