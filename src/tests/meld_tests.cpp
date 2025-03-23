#include <game/meld.h>
#include <tests/tests_declaration.h>
#include <tests/test.h>

namespace GameTest {

    void invalidTile() {
        const int invalidValuesCount = 4;
        int exceptionCount = 0;
        int invalidValues[invalidValuesCount] = {-1, 0, 14, 15};
        
        for (int i = 0; i < invalidValuesCount; i++) {
            try {
                Tile* tile = new Tile(invalidValues[i], Color::BLACK, false);
            }
            catch (const std::invalid_argument&) {
                exceptionCount++;
            }
        }

        Test::validate(exceptionCount == invalidValuesCount, __func__);
    }

    void validSet() {
        IMeld* meld = new Meld();
        meld->add(new Tile(1, Color::YELLOW, false));
        meld->add(new Tile(1, Color::RED, false));
        meld->add(new Tile(1, Color::BLACK, false));
        Test::validate(meld->getType() == MeldType::SET && meld->isValid(), __func__);
        meld->add(new Tile(1, Color::BLUE, false));
        Test::validate(meld->getType() == MeldType::SET && meld->isValid(), __func__);
        delete meld;
    }

    void invalidSet() {
        IMeld* meld = new Meld();
        meld->add(new Tile(2, Color::YELLOW, false));
        meld->add(new Tile(2, Color::RED, false));
        meld->add(new Tile(2, Color::RED, false));
        Test::validate(meld->getType() == MeldType::INVALID && !meld->isValid(), __func__);
        delete meld;
    }

    void validSequence() {
        IMeld* meld = new Meld();
        meld->add(new Tile(10, Color::YELLOW, false));
        meld->add(new Tile(11, Color::YELLOW, false));
        meld->add(new Tile(12, Color::YELLOW, false));
        Test::validate(meld->getType() == MeldType::SEQUENCE && meld->isValid(), __func__);
        delete meld;
    }

    void validFullSequence() {
        IMeld* meld = new Meld();
        for (int i = 1; i <= 13; i++) {
            meld->add(new Tile(i, Color::YELLOW, false));
        }
        Test::validate(meld->getType() == MeldType::SEQUENCE && meld->isValid(), __func__);
        delete meld;
    }

    void invalidSequence() {
        IMeld* meld = new Meld();
        meld->add(new Tile(1, Color::YELLOW, false));
        meld->add(new Tile(3, Color::YELLOW, false));
        meld->add(new Tile(4, Color::YELLOW, false));
        Test::validate(meld->getType() == MeldType::INVALID && !meld->isValid(), __func__);
        delete meld;
    }

    void oneJokerSet() {
        IMeld* meld = new Meld();
        meld->add(new Tile(5, Color::YELLOW, false));
        meld->add(new Tile(5, Color::RED, false));
        meld->add(new Tile(0, Color::BLACK, true));
        Test::validate(meld->getType() == MeldType::SET && meld->isValid(), __func__);
        delete meld;
    }

    void twoJokerSet() {
        IMeld* meld = new Meld();
        meld->add(new Tile(0, Color::RED, true));
        meld->add(new Tile(0, Color::RED, true));
        meld->add(new Tile(2, Color::RED, false));
        Test::validate(meld->getType() == MeldType::SET && meld->isValid(), __func__);
        delete meld;
    }

    void twoJokerSequence() {
        IMeld* meld = new Meld();
        meld->add(new Tile(0, Color::BLACK, true));
        meld->add(new Tile(0, Color::RED, true));
        meld->add(new Tile(5, Color::YELLOW, false));
        Test::validate(meld->getType() == MeldType::SEQUENCE && meld->isValid(), __func__);
        delete meld;
    }

    void jokerSequence() {
        IMeld* meld = new Meld();
        meld->add(new Tile(3, Color::YELLOW, true));
        meld->add(new Tile(0, Color::BLACK, true));
        meld->add(new Tile(5, Color::YELLOW, false));
        meld->add(new Tile(6, Color::YELLOW, false));
        Test::validate(meld->getType() == MeldType::SEQUENCE && meld->isValid(), __func__);
        delete meld;
    }

    void runMeldTests() {
        invalidTile();
        validSet();
        invalidSet();
        validSequence();
        validFullSequence();
        invalidSequence();
        oneJokerSet();
        twoJokerSet();
        twoJokerSequence();
        jokerSequence();
    }
}
