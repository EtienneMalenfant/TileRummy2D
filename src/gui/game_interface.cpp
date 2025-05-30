#include <gui/game_interface.h>
#include <gui/game_event_handler.h>
#include <gui/settings.h>

using namespace gui;

WindowGameInterface::WindowGameInterface(GameDependencies* dependencies, const AppSettings& appSettings) : _appSettings(&appSettings) {
    GameEventHandler* eventsListenerHandler = new GameEventHandler();
    // si le joueur est un utilisateur
    if (dependencies->playerController != nullptr) {
        _playerControlsProxy = new PlayerControlsProxy(eventsListenerHandler, dependencies->playerController,
            dependencies->currentPlayer->getName(), eventsListenerHandler);
        _gameEventListener = _playerControlsProxy;
    }
    else {
        _gameEventListener = eventsListenerHandler;
    }

    _gameEventHandler = eventsListenerHandler;
    // pour ajouter mon proxy dans IPlayerController
    _dependencies = new GameDependencies(dependencies->eventPublisher, dependencies->meldsContainer, _playerControlsProxy,
        dependencies->currentPlayer, dependencies->players);
}

WindowGameInterface::~WindowGameInterface() {
    delete _window;
    delete _gameEventListener;
    delete _windowFactory;
    delete _windowSettings;
    delete _dependencies;
}

IGameEventListener* WindowGameInterface::getEventListener() const {
    return _gameEventListener;
}

void WindowGameInterface::start() {
    // Créer la fenêtre
    _windowSettings = new WindowSettings(1600, 900, true, false, "Rummy");
    _windowFactory = new WindowFactory(_dependencies, _gameEventHandler, _windowSettings, _appSettings);
    _window = _windowFactory->createWindow();

    if (_playerControlsProxy != nullptr) {
        _playerControlsProxy->setGameLogger(_gameEventHandler->getLogger());
    }
}

bool WindowGameInterface::isRunning() const {
    return _window->isOpen();
}

void WindowGameInterface::refresh() {
    _window->handleEvents();
    _window->clear();
    _gameEventHandler->handleEvents();
    _window->draw();
}
