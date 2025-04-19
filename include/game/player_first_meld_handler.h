#pragma once
#include <game/player.h>

class PlayerFirstMeldHandler : public IPlayerController {
    private:
        bool _hasStarted {false};
        IPlayerController* _controller;
        IPlayer* _player;
        int _points {0};
        std::vector<const Tile*> _playedTiles;

        const static int POINTS_REQUIRED = 30;
        const Tile* getTile(int tileId);
        bool isPlayingOnHisTiles(Action* action);
    public :
        PlayerFirstMeldHandler(IPlayerController* controller, IPlayer* player);
        ~PlayerFirstMeldHandler() override;
        void draw() override;
        bool addAction(Action* action) override;
        bool commitActions() override;
        void cancelActions() override;
        bool isStarted() const override;
    };
