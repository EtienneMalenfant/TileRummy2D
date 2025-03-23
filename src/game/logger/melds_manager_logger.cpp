#include <game/logger/melds_manager_logger.h>

MeldsManagerLogger::MeldsManagerLogger(IMeldsManager* meldsManager, ILogger* logger)
    : _meldsManager(meldsManager), _logger(logger) 
{
}

MeldsManagerLogger::~MeldsManagerLogger() {
    delete _meldsManager;
}

bool MeldsManagerLogger::confirm() {
    bool hasConfirmed = _meldsManager->confirm();
    _logger->log("MeldsManager::confirm = " + std::to_string(hasConfirmed));
    return hasConfirmed;
}

void MeldsManagerLogger::cancel() {
    _logger->log("MeldsManager::cancel");
    _meldsManager->cancel();
}

void MeldsManagerLogger::moveTile(Action* action) {
    _logger->log("MeldsManager::moveTile " + action->toString());
    _meldsManager->moveTile(action);
}

void MeldsManagerLogger::putTile(Action* action, Tile* tile) {
    _logger->log("MeldsManager::putTile " + action->toString());
    _meldsManager->putTile(action, tile);
}

