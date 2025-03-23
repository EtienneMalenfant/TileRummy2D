#include <tests/test.h>
#include <tests/tests_declaration.h>

namespace GameTest {

    IMeldsManager* meldsManager;
    IMeldsContainer* meldsContainer;

    void createMeldManager() {
        MeldsManager* meldsManagerObj = new MeldsManager();
        meldsManager = meldsManagerObj;
        meldsContainer = meldsManagerObj;
    }

    void buildMelds(IMeldsManager* meldsManager) {
        meldsManager->putTile(new Action {0, -1, -1}, new Tile(11, Color::RED, false, 0));
        meldsManager->putTile(new Action {1, 0, -1}, new Tile(1, Color::BLACK, true, 1));
        meldsManager->putTile(new Action {2, 1, -1}, new Tile(11, Color::BLUE, false, 2));
        Test::validate(meldsManager->confirm(), __func__);

        meldsManager->putTile(new Action {3, -1, -1}, new Tile(3, Color::RED, false, 3));
        meldsManager->putTile(new Action {4, 3, -1}, new Tile(3, Color::YELLOW, false, 4));
        meldsManager->putTile(new Action {5, 4, -1}, new Tile(3, Color::BLUE, false, 5));
        meldsManager->putTile(new Action {6, 5, -1}, new Tile(3, Color::BLACK, false, 6));
        Test::validate(meldsManager->confirm(), __func__);

        meldsManager->putTile(new Action {7, -1, -1}, new Tile(5, Color::YELLOW, false, 7));
        meldsManager->putTile(new Action {8, 7, -1}, new Tile(6, Color::YELLOW, false, 8));
        meldsManager->putTile(new Action {9, 8, -1}, new Tile(7, Color::YELLOW, false, 9));
        meldsManager->putTile(new Action {10, 9, -1}, new Tile(8, Color::YELLOW, false, 10));
        meldsManager->putTile(new Action {11, 10, -1}, new Tile(9, Color::YELLOW, false, 11));
        meldsManager->putTile(new Action {12, 11, -1}, new Tile(10, Color::YELLOW, false, 12));
        meldsManager->putTile(new Action {13, 12, -1}, new Tile(11, Color::YELLOW, false, 13));
        Test::validate(meldsManager->confirm(), __func__);
    }

    void splitMeldAndCancel() {
        meldsManager->moveTile(new Action {10, -1, -1});
        // 3 melds existant + 1 qui s'est split + le nouveau avec la tuile seule
        Test::validate(meldsContainer->getMelds()->size() == 5, __func__);
        // tuile seule, donc invalide
        Test::validate(meldsManager->confirm() == false, __func__);

        // retour au dernier état valide
        meldsManager->cancel();
        Test::validate(meldsContainer->getMelds()->size() == 3, __func__);
        Test::validate(meldsManager->confirm(), __func__);
    }

    void emptyMeldDeletion() {
        meldsManager->moveTile(new Action {13, -1, -1});
        Test::validate(meldsContainer->getMelds()->size() == 4, __func__);
        meldsManager->moveTile(new Action {13, 2, -1});
        Test::validate(meldsContainer->getMelds()->size() == 3, __func__);
        Test::validate(meldsManager->confirm(), __func__);
    }

    void nukeMeldsThenCancel() {
        meldsManager->moveTile(new Action {12, -1, -1});
        meldsManager->moveTile(new Action {11, -1, -1});
        meldsManager->moveTile(new Action {1, 12, -1});
        meldsManager->moveTile(new Action {2, 11, -1});
        meldsManager->moveTile(new Action {0, -1, 7});
        meldsManager->moveTile(new Action {5, -1, 9});
        Test::validate(meldsManager->confirm() == false, __func__);
        meldsManager->cancel();
        Test::validate(meldsContainer->getMelds()->size() == 3, __func__);
        Test::validate(meldsManager->confirm(), __func__);
    }

    void runMeldManagerTests() {
        createMeldManager();
        buildMelds(meldsManager);
        splitMeldAndCancel();
        emptyMeldDeletion();
        nukeMeldsThenCancel();
        delete meldsManager;
    }

}