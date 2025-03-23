#include <tests/test.h>
#include <tests/tests_declaration.h>
#include <bot/actions_analyser.h>
#include <bot/smart_analyser/smart_insertions_analyser.h>

using namespace bot;

namespace ActionsAnalyserTest {

    IMeldsContainer* meldsContainer;
    IMeldsManager* meldsManager;
    std::list<const Tile*>* tiles;

    void init() {
        MeldsManager* meldsManagerObj = new MeldsManager();
        meldsManager = meldsManagerObj;
        meldsContainer = meldsManagerObj;
        GameTest::buildMelds(meldsManager); // voir meld_manager_tests.cpp
        tiles = new std::list<const Tile*>();
    }

    void emptyTiles() {
        for (const Tile* tile : *tiles) {
            delete tile;
        }
        tiles->clear();
    }

    void addTiles() {
        tiles->push_back(new Tile(1, Color::RED, false, 30));
        tiles->push_back(new Tile(2, Color::RED, false, 31));
        tiles->push_back(new Tile(3, Color::RED, false, 32));
        tiles->push_back(new Tile(4, Color::RED, false, 33));
        tiles->push_back(new Tile(5, Color::RED, false, 34));

        tiles->push_back(new Tile(10, Color::RED, false, 35));
        tiles->push_back(new Tile(10, Color::YELLOW, false, 36));
        tiles->push_back(new Tile(10, Color::BLACK, false, 37));

        tiles->push_back(new Tile(9, Color::BLACK, false, 38));
        tiles->push_back(new Tile(9, Color::YELLOW, false, 39));
        tiles->push_back(new Tile(0, Color::BLACK, true, 40));
    }

    void detectNewMelds() {
        IActionsAnalyser* analyser = new NewMeldsAnalyser();
        emptyTiles();
        addTiles();

        std::list<std::list<Action*>*>* newMeldsActions = analyser->getActionsSequences(tiles, true);
        Test::validate(newMeldsActions->size() == 3, __func__);

        int longuestMeld = 0;
        for (auto meldActions : *newMeldsActions) {
            longuestMeld = std::max(longuestMeld, (int)meldActions->size());
        }
        Test::validate(longuestMeld == 5, __func__);

        delete newMeldsActions;
        delete analyser;
    }

    void detectInsertions() {
        IActionsAnalyser* analyser = new DumbActionsAnalyser(meldsContainer);
        std::list<std::list<Action*>*>* insertions = analyser->getActionsSequences(tiles, true);
        // le 9 jaune + 2 déplacements = 3
        Test::validate(insertions->size() == 1 && insertions->front()->size() == 3, __func__);
        delete insertions->front();
        delete insertions;
        emptyTiles();

        // insertion simple
        tiles->push_back(new Tile(12, Color::YELLOW, false, 41));
        insertions = analyser->getActionsSequences(tiles);
        Test::validate(insertions->size() == 1 && insertions->front()->size() == 1, __func__);
        delete insertions->front();
        delete insertions;
        delete analyser;
    }

    void useTileOnce() { // vérifier qu'une même tuile n'est pas utilisé 2 fois
        IActionsAnalyser* analyser = new NewMeldsAnalyser();
        emptyTiles();
        tiles->push_back(new Tile(3, Color::RED, false, 32));
        tiles->push_back(new Tile(4, Color::RED, false, 33));
        tiles->push_back(new Tile(5, Color::RED, false, 34));

        tiles->push_back(new Tile(5, Color::YELLOW, false, 35));
        tiles->push_back(new Tile(5, Color::BLACK, false, 36));

        // il pourrait détecter 2,3,4,5 rouge, ou 5r, 5j, 5n, mais pas les deux
        std::list<std::list<Action*>*>* newMeldsActions = analyser->getActionsSequences(tiles, true);
        Test::validate(newMeldsActions->size() == 1, __func__);
        delete analyser;
    }


    void runTests() {
        init();
        // l'ordre est important
        detectNewMelds();
        detectInsertions();
        useTileOnce();
        emptyTiles();
        delete meldsManager;
        delete tiles;
    }
}

// void buildMelds(IMeldsManager* meldsManager) {
//     meldsManager->putTile(new Action {0, -1, -1}, new Tile(11, Color::RED, false, 0));
//     meldsManager->putTile(new Action {1, 0, -1}, new Tile(1, Color::BLACK, true, 1));
//     meldsManager->putTile(new Action {2, 1, -1}, new Tile(11, Color::BLUE, false, 2));
//     Test::validate(meldsManager->confirm(), __func__);

//     meldsManager->putTile(new Action {3, -1, -1}, new Tile(3, Color::RED, false, 3));
//     meldsManager->putTile(new Action {4, 3, -1}, new Tile(3, Color::YELLOW, false, 4));
//     meldsManager->putTile(new Action {5, 4, -1}, new Tile(3, Color::BLUE, false, 5));
//     meldsManager->putTile(new Action {6, 5, -1}, new Tile(3, Color::BLACK, false, 6));
//     Test::validate(meldsManager->confirm(), __func__);

//     meldsManager->putTile(new Action {7, -1, -1}, new Tile(5, Color::YELLOW, false, 7));
//     meldsManager->putTile(new Action {8, 7, -1}, new Tile(6, Color::YELLOW, false, 8));
//     meldsManager->putTile(new Action {9, 8, -1}, new Tile(7, Color::YELLOW, false, 9));
//     meldsManager->putTile(new Action {10, 9, -1}, new Tile(8, Color::YELLOW, false, 10));
//     meldsManager->putTile(new Action {11, 10, -1}, new Tile(9, Color::YELLOW, false, 11));
//     meldsManager->putTile(new Action {12, 11, -1}, new Tile(10, Color::YELLOW, false, 12));
//     meldsManager->putTile(new Action {13, 12, -1}, new Tile(11, Color::YELLOW, false, 13));
//     Test::validate(meldsManager->confirm(), __func__);
// }
