#pragma once
#include <game/player.h>
#include <logger.h>
#include <string>

class PlayerControllerLogger : public IPlayerController {
private:
    IPlayerController* _controller;
    std::string _playerName;
    ILogger* _logger;
public:
    PlayerControllerLogger(IPlayerController* controller, std::string playerName, ILogger* logger);
    ~PlayerControllerLogger();
    void draw() override;
    bool addAction(Action* action) override;
    bool commitActions() override;
    void cancelActions() override;
    bool isStarted() const override;
};
