#include <bot/actions_analyser.h>
#include <bot/smart_analyser/meld_builder.h>

namespace bot {

    class SmartInsertionsAnalyser : public IActionsAnalyser {
    private:
        ITileWrapperManager* _tileWrapperManager;
        ITileGraph* _tileGraph;
        IMeldBuilder* _meldBuilder;
    public:
        SmartInsertionsAnalyser(IMeldsContainer* meldContainer);
        ~SmartInsertionsAnalyser() override;
        std::list<std::list<Action*>*>* getActionsSequences(const std::list<const Tile*>* playerTiles, bool useJoker) override;
    };
};
