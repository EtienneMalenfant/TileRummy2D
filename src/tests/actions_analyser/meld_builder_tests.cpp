#include <bot/smart_analyser/meld_builder.h>
#include <tests/test.h>

using namespace bot;

namespace ActionsAnalyserTest {

    namespace SmartUtils {

        MeldsManager* meldsManager = new MeldsManager();
        IMeldsContainer* meldsContainer = meldsManager;
        TileWrapperManager* tileWrapperManager = new TileWrapperManager(meldsContainer);
        std::list<const Tile*>* playerTiles = new std::list<const Tile*>();
        TileGraph* tileGraph = new TileGraph();
        IMeldBuilder* meldBuilder = new MeldBuilder();

        void init() {
            meldsManager->putTile(new Action{1, -1, -1}, new Tile(5, Color::RED, false, 1));
            meldsManager->putTile(new Action{2, 1, -1}, new Tile(5, Color::BLUE, false, 2));
            meldsManager->putTile(new Action{3, 2, -1}, new Tile(5, Color::YELLOW, false, 3));
            // meldsManager->putTile(new Action{4, 3, -1}, new Tile(5, Color::BLACK, false, 4));

            meldsManager->putTile(new Action{5, -1, -1}, new Tile(8, Color::RED, false, 5));
            meldsManager->putTile(new Action{6, 5, -1}, new Tile(9, Color::RED, false, 6));
            meldsManager->putTile(new Action{7, 6, -1}, new Tile(10, Color::RED, false, 7));
            meldsManager->putTile(new Action{8, 7, -1}, new Tile(11, Color::RED, false, 8));
            meldsManager->putTile(new Action{9, 8, -1}, new Tile(12, Color::RED, false, 9));
            // meldsManager->putTile(new Action{10, 9, -1}, new Tile(13, Color::RED, false, 10));

            playerTiles->push_back(new Tile(5, Color::RED, false, 11));
            playerTiles->push_back(new Tile(5, Color::BLACK, false, 12));

            playerTiles->push_back(new Tile(9, Color::RED, false, 13));
            // playerTiles->push_back(new Tile(10, Color::RED, false, 14));
            playerTiles->push_back(new Tile(13, Color::RED, false, 15));

            tileWrapperManager->refresh(playerTiles);
        }

        void clearPlayerTiles(std::list<const Tile*>* playerTiles) {
            for (const Tile* tile : *playerTiles) {
                delete tile;
            }
            playerTiles->clear();
        }

        void cleanActionsLists(std::list<std::list<Action*>*>* actionsLists) {
            for (std::list<Action*>* actions : *actionsLists) {
                for (Action* action : *actions) {
                    delete action;
                }
                delete actions;
            }
            delete actionsLists;
        }

        void buildTest() {
            tileGraph->build(tileWrapperManager); // testé en debug
            const std::vector<ITileNode*> playerTileNodes = tileGraph->getPlayerTileNodes();
            Test::validate(playerTileNodes.size() == 4, "TileGraph playerTileNodes size");
        }

        void simpleInsertionTest() {
            Test::validate(meldsManager->confirm(), __func__);
            IGameBuildState* gameState = meldBuilder->buildGame(tileGraph, false);
            Test::validate(gameState->getPlayerTileCount() == 2, __func__);
            std::list<std::list<Action*>*>* actionsLists = gameState->toActions();
            Test::validate(actionsLists->size() == 2, __func__);

            for (std::list<Action*>* actions : *actionsLists) {
                Test::validate(actions->size() == 1, __func__);
                Action* action = actions->front();
                Test::validate(action->toString() == Action{12, 3, -1}.toString() || action->toString() == Action{15, 9, -1}.toString(), __func__);
            }

            cleanActionsLists(actionsLists);
        }

        void middleInsertionTest() {
            clearPlayerTiles(playerTiles);
            playerTiles->push_back(new Tile(9, Color::RED, false, 13));
            playerTiles->push_back(new Tile(10, Color::RED, false, 14));

            Test::validate(meldsManager->confirm(), __func__);

            tileWrapperManager->refresh(playerTiles);
            tileGraph->build(tileWrapperManager);
            IGameBuildState* gameState = meldBuilder->buildGame(tileGraph, false);
            Test::validate(gameState->getPlayerTileCount() == 2, __func__);

            std::list<std::list<Action*>*>* actionsLists = gameState->toActions();
            std::list<Action*>* actions = actionsLists->front();
            Test::validate(actionsLists->size() == 1, __func__);
            Test::validate(actions->size() == 4, __func__);

            Action expectedActions[4] = {Action{13, -1, -1}, Action{14, 13, -1}, Action{8, 14, -1}, Action{9, 8, -1}};
            for (int i = 0; i < 4; i++) {
                Action* action = actions->front();
                actions->pop_front();
                Test::validate(action->toString() == expectedActions[i].toString(), __func__);
                delete action;
            }
            delete actions;
            delete actionsLists;
        }

        void setFormationWithGameTilesTest() {
            clearPlayerTiles(playerTiles);
            playerTiles->push_back(new Tile(5, Color::RED, false, 50));

            meldsManager->putTile(new Action{4, 3, -1}, new Tile(5, Color::BLACK, false, 4));
            meldsManager->putTile(new Action{51, -1, -1}, new Tile(5, Color::YELLOW, false, 51));
            meldsManager->putTile(new Action{52, 51, -1}, new Tile(6, Color::YELLOW, false, 52));
            meldsManager->putTile(new Action{53, 52, -1}, new Tile(7, Color::YELLOW, false, 53));
            meldsManager->putTile(new Action{54, 53, -1}, new Tile(8, Color::YELLOW, false, 54));

            tileWrapperManager->refresh(playerTiles);
            tileGraph->build(tileWrapperManager);
            IGameBuildState* gameState = meldBuilder->buildGame(tileGraph, false);
            Test::validate(gameState->getPlayerTileCount() == 1, __func__);

            std::list<std::list<Action*>*>* actionsLists = gameState->toActions();
            Test::validate(actionsLists->size() == 1, __func__);
            std::list<Action*>* actions = actionsLists->front();
            Test::validate(actions->size() == 3, __func__);

            cleanActionsLists(actionsLists);
        }

        void sequenceBothWaysTest() {
            delete meldsManager;
            meldsManager = new MeldsManager();
            meldsContainer = meldsManager;
            delete tileWrapperManager;
            tileWrapperManager = new TileWrapperManager(meldsContainer);

            meldsManager->putTile(new Action{1, -1, -1}, new Tile(5, Color::RED, false, 1));
            meldsManager->putTile(new Action{2, 1, -1}, new Tile(5, Color::BLUE, false, 2));
            meldsManager->putTile(new Action{3, 2, -1}, new Tile(5, Color::YELLOW, false, 3));
            meldsManager->putTile(new Action{4, 3, -1}, new Tile(5, Color::BLACK, false, 4));

            meldsManager->putTile(new Action{60, -1, -1}, new Tile(7, Color::RED, false, 60));
            meldsManager->putTile(new Action{61, 60, -1}, new Tile(7, Color::BLACK, false, 61));
            meldsManager->putTile(new Action{62, 61, -1}, new Tile(7, Color::YELLOW, false, 62));
            meldsManager->putTile(new Action{63, 62, -1}, new Tile(7, Color::BLUE, false, 63));


            clearPlayerTiles(playerTiles);
            playerTiles->push_back(new Tile(6, Color::RED, false, 59));

            tileWrapperManager->refresh(playerTiles);
            tileGraph->build(tileWrapperManager);
            IGameBuildState* gameState = meldBuilder->buildGame(tileGraph, false);
            Test::validate(gameState->getPlayerTileCount() == 1, __func__);

            std::list<std::list<Action*>*>* actionsLists = gameState->toActions();
            Test::validate(actionsLists->size() == 1, __func__);
            std::list<Action*>* actions = actionsLists->front();
            Test::validate(actions->size() == 3, __func__);

            cleanActionsLists(actionsLists);
        }

        void sequenceBothWaysTestInvalid() {
            meldsManager->putTile(new Action{10, -1, -1}, new Tile(9, Color::RED, false, 10));
            meldsManager->putTile(new Action{11, -1, -1}, new Tile(10, Color::RED, false, 11));
            meldsManager->putTile(new Action{12, 11, -1}, new Tile(11, Color::RED, false, 12));
            meldsManager->putTile(new Action{13, 12, -1}, new Tile(12, Color::RED, false, 13));

            clearPlayerTiles(playerTiles);
            playerTiles->push_back(new Tile(10, Color::RED, false, 99));

            tileWrapperManager->refresh(playerTiles);
            tileGraph->build(tileWrapperManager);
            IGameBuildState* gameState = meldBuilder->buildGame(tileGraph, false);
            Test::validate(gameState->getPlayerTileCount() == 0, __func__);
        }

        void useJokerTest() {
            clearPlayerTiles(playerTiles);
            playerTiles->push_back(new Tile(7, Color::RED, false, 50));
            playerTiles->push_back(new Tile(0, Color::BLACK, true, 14));

            tileWrapperManager->refresh(playerTiles);
            tileGraph->build(tileWrapperManager);
            IGameBuildState* gameState = meldBuilder->buildGame(tileGraph, true);
            Test::validate(gameState->getPlayerTileCount() == 2, __func__);
            std::list<std::list<Action*>*>* actionsLists = gameState->toActions();
            Test::validate(actionsLists->size() == 1, __func__);
            std::list<Action*>* actions = actionsLists->front();
            Test::validate(actions->size() == 3, __func__);
            cleanActionsLists(actionsLists);
        }

        void runMeldBuilderTests() {
            init();
            buildTest();
            simpleInsertionTest();
            middleInsertionTest();
            setFormationWithGameTilesTest();
            sequenceBothWaysTest();
            sequenceBothWaysTestInvalid();
            useJokerTest();

            for (const Tile* tile : *playerTiles) {
                delete tile;
            }
            delete playerTiles;
            delete tileWrapperManager;
            delete tileGraph;
            delete meldsManager;
        }
    }
}
