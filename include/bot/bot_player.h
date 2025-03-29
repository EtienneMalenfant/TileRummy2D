#pragma once
#include <game/player.h>
#include <bot/actions_analyser.h>
#include <random>

namespace bot {

    class IBotPlayer {
        public:
        virtual void setWaitTime(unsigned int time) = 0;
        virtual ~IBotPlayer() = default;
    };

    class BotPlayer : public IGameEventListener, public IBotPlayer {
    private:
        IPlayerController* _controller;
        IPlayer* _player;
        IActionsAnalyser* _newMeldsAnalyser;
        IActionsAnalyser* _insertionsAnalyser;
        bool _useJoker {false};

        std::list<std::list<Action*>*>* _newMelds {};

        std::mt19937 _randGen {std::random_device()()};
        unsigned int _baseWaitTime {};
        void simulateProcessing();
        void deleteAndClearNewMelds();
        std::list<std::list<Action*>*>* getNewMelds();
        void handleFirstTurn();
        bool playAllNewMelds();
        void playActionList(std::list<Action*>* actionList);
        std::list<const Tile*>* filterStandbyTiles();
        bool playSomething();
        bool playInsertions();
        public:
        BotPlayer(IPlayerController* controller, IPlayer* player, IActionsAnalyser* actionsAnalyser);
        ~BotPlayer() override;

        void update(ptr<IEvent> event) override;
        void setWaitTime(unsigned int time) override;
    };
};
