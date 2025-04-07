#pragma once
#include <queue>

#include <game/action.h>
#include <game/melds_manager.h>

namespace bot {

    class IActionsAnalyser {
    public :
        virtual std::list<std::list<Action*>*>* getActionsSequences(const std::list<const Tile*>* playerTiles, bool useJoker = true) = 0;
        virtual ~IActionsAnalyser() = default;
    };

    class BaseActionsAnalyser : public IActionsAnalyser {
    protected:
        const Tile* _tiles[4][13] {nullptr};
        std::list<const Tile*> _standbyTiles {};
        const Tile* _joker[2] {nullptr};
        int _jokerCount {0};

        void resetPlayerTiles();
        void fillStandbyTiles(const std::list<const Tile*>* playerTiles);
        void updateTilesArray(); // remplit les tableau de tuiles avec _standbyTiles
        void emptyJokers();
    public:
        BaseActionsAnalyser() = default;
    };

    class NewMeldsAnalyser : public BaseActionsAnalyser {
    private:
        void getNewSets(std::list<std::list<Action*>*>* newMelds);
        void getNewSequences(std::list<std::list<Action*>*>* newMelds);
        void createSequence(std::list<std::list<Action*>*>* newMelds, const std::list<const Tile*>& sequence);

    public:
        NewMeldsAnalyser() = default;
        std::list<std::list<Action*>*>* getActionsSequences(const std::list<const Tile*>* playerTiles, bool useJoker = true) override;

    };

    class DumbActionsAnalyser : public BaseActionsAnalyser {
    private:
        IMeldsContainer* _meldsContainer;
        std::list<const IMeld*> _sequences {};
        std::list<const IMeld*> _sets {};
        std::list<const IMeld*> _invalids {};

        void sortMelds();
        void playOnSequences(std::list<std::list<Action*>*>* insertions);
        void handleMiddleInsertions(std::list<std::list<Action*>*>* insertions, const IMeld* sequence);
        void playOnSets(std::list<std::list<Action*>*>* insertions);
        void removeSameTiles(std::list<std::list<Action*>*>* insertions);
        void reset();
    public:
        DumbActionsAnalyser(IMeldsContainer* meldsContainer);
        std::list<std::list<Action*>*>* getActionsSequences(const std::list<const Tile*>* playerTiles, bool useJoker = true) override;
    };
};
