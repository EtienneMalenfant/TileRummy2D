#include <bot/smart_analyser/tile_node.h>
#include <bot/smart_analyser/tile_node_iterator.h>
#include <bot/smart_analyser/tile_group.h>

namespace bot {

    class IGameBuildState {
    public:
        virtual bool contains(ITileNode* tileNode) = 0;
        virtual int getPlayerTileCount() = 0;
        virtual void add(ITileGroup* group) = 0;
        virtual std::list<std::list<Action*>*>* toActions() = 0;

        virtual ~IGameBuildState() = default;
    };

    class GameBuildState : public IGameBuildState {
    private:
        int playerTileCount = 0;
        std::vector<ITileGroup*> _groups;
    public:
        GameBuildState() = default;
        ~GameBuildState() override;
        bool contains(ITileNode* tileNode) override;
        int getPlayerTileCount() override;
        void add(ITileGroup* group) override;
        std::list<std::list<Action*>*>* toActions() override;
    };

}
