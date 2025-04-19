#include <game/logger/player_controller_logger.h>

PlayerControllerLogger::PlayerControllerLogger(IPlayerController* controller, std::string playerName, ILogger* logger)
    : _controller(controller), _playerName(playerName), _logger(logger)
{}

PlayerControllerLogger::~PlayerControllerLogger() {
    delete _controller;
}

void PlayerControllerLogger::draw() {
    _logger->log("PlayerController::draw");
    _logger->log(_playerName + " played", LogType::Info);
    _controller->draw();
}

bool PlayerControllerLogger::addAction(Action* action) {
    _logger->log("PlayerController::addAction");
    return _controller->addAction(action);
}

bool PlayerControllerLogger::commitActions() {
    _logger->log("PlayerController::commitActions");
    bool played = _controller->commitActions();
    if (played) {
        _logger->log(_playerName + " played", LogType::Info);
    }
    return played;
}

void PlayerControllerLogger::cancelActions() {
    _logger->log("PlayerController::cancelActions");
    _controller->cancelActions();
}

bool PlayerControllerLogger::isStarted() const {
    return _controller->isStarted();
}
