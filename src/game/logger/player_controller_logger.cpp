#include <game/logger/player_controller_logger.h>

PlayerControllerLogger::PlayerControllerLogger(IPlayerController* controller, ILogger* logger)
    : _controller(controller), _logger(logger)
{}

PlayerControllerLogger::~PlayerControllerLogger() {
    delete _controller;
}

void PlayerControllerLogger::draw() {
    _logger->log("PlayerController::draw");
    _controller->draw();
}

bool PlayerControllerLogger::addAction(Action* action) {
    _logger->log("PlayerController::addAction");
    return _controller->addAction(action);
}

bool PlayerControllerLogger::commitActions() {
    _logger->log("PlayerController::commitActions");
    return _controller->commitActions();
}

void PlayerControllerLogger::cancelActions() {
    _logger->log("PlayerController::cancelActions");
    _controller->cancelActions();
}

bool PlayerControllerLogger::isStarted() const {
    return _controller->isStarted();
}
