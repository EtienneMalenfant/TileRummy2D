#include <gui/player_controls_proxy.h>
#include <thread>

using namespace gui;

PlayerControlsProxy::PlayerControlsProxy(IGameEventListener* eventListener, IPlayerController* controller,
     const std::string& userName, IUpdateable* rootUpdateable)
    : _eventListener(eventListener), _controller(controller), _userName(userName), _rootUpdateable(rootUpdateable) {}

PlayerControlsProxy::~PlayerControlsProxy() {
    // sinon on peut bloquer ici et ne jamais sortir
    _isUserTurn = false;
    _cv.notify_all();
    // laisser le temps au thread de se débloquer
    // sinon on va accéder à cet objet qui est delete en revenant (ligne _cv.wait ...)
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    delete _eventListener;
}

void PlayerControlsProxy::displayNotYourTurnMessage() {
    _gameLogger->log("Ce n'est pas votre tour", LogType::Warning);
}

// ------------------------------

void PlayerControlsProxy::setGameLogger(ILogger* logger) {
    _gameLogger = logger;
}

// ------------------------------

void PlayerControlsProxy::update(ptr<IEvent> event) {
    _eventListener->update(event);
    if (event->getType() == EventType::GIVE_TURN && event->toString() == _userName) {
        _gameLogger->log("C'est à ton tour", LogType::Important);
        _isUserTurn = true;
        // Attendre que notre tour soit finit pour continuer dans ce thread (celui du jeu)
        std::unique_lock<std::mutex> lock(_mutex);
        _cv.wait(lock, [this] { return !_isUserTurn; });
    }
}

// ------------------------------

void PlayerControlsProxy::draw() {
    if (_isUserTurn) {
        _controller->draw();
        _isUserTurn = false;
        _cv.notify_one();
    } else {
        displayNotYourTurnMessage();
    }
}

bool PlayerControlsProxy::addAction(Action* action) {
    if (_isUserTurn) {
        return _controller->addAction(action);
    }
    displayNotYourTurnMessage();
    return false;
}

bool PlayerControlsProxy::commitActions() {
    if (_isUserTurn) {
        bool hasCommitedActions = _controller->commitActions();
        if (hasCommitedActions) {
            _isUserTurn = false;
            _cv.notify_one();
        }
        else {
            _gameLogger->log("Le jeu n'est pas valide", LogType::Warning);
            if (_controller->isStarted() == false) {
                _gameLogger->log("Vous devez jouer au moins 30 pts", LogType::Warning);
            }
        }
        return hasCommitedActions;
    }
    displayNotYourTurnMessage();
    return false;
}

void PlayerControlsProxy::cancelActions() {
    if (_isUserTurn) {
        _controller->cancelActions();
        _gameLogger->log("Vos mouvements ont été annulés", LogType::Info);
        _rootUpdateable->update();
    } else {
        _gameLogger->log("Ce n'est pas votre tour", LogType::Warning);
    }
}

bool PlayerControlsProxy::isStarted() const {
    return _controller->isStarted();
}

