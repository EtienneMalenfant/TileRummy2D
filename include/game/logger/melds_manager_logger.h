#pragma once
#include <game/melds_manager.h>
#include <logger.h>

class MeldsManagerLogger : public IMeldsManager {
private:
    IMeldsManager* _meldsManager;
    ILogger* _logger;
public:
    MeldsManagerLogger(IMeldsManager* meldsManager, ILogger* logger);
    ~MeldsManagerLogger();
    bool confirm() override;
    void cancel() override;
    void moveTile(Action* action) override;
    void putTile(Action* action, Tile* tile) override;
};
