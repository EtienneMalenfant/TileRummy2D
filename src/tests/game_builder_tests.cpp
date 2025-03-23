#include <tests/test.h>

#include <game_builder.h>

namespace GameBuilderTest {

    
    IGameBuilder* gameBuilder;
    
    void emptyGameBuilder() {
        Test::validate(gameBuilder->getGame() == nullptr, __func__);
        Test::validate(gameBuilder->getDependencies() == nullptr, __func__);
    }
    
    void notEnoughPlayers() {
        gameBuilder->addBot("Alice");
        Test::validate(gameBuilder->getGame() == nullptr, __func__);
        Test::validate(gameBuilder->getDependencies() == nullptr, __func__);
    }
    
    void validGame() {
        gameBuilder->addBot("Bob");
        Test::validate(gameBuilder->getGame() != nullptr, __func__);
        Test::validate(gameBuilder->getDependencies() != nullptr, __func__);
    }
    
    void tooMuchPlayers() {
        gameBuilder->addBot("Charlie");
        gameBuilder->addBot("Dad");
        gameBuilder->addBot("Ethan");
        Test::validate(gameBuilder->getGame() == nullptr, __func__);
        Test::validate(gameBuilder->getDependencies() == nullptr, __func__);
    }

    void resetBuilder() {
        gameBuilder->reset();
        Test::validate(gameBuilder->getGame() == nullptr, __func__);
        Test::validate(gameBuilder->getDependencies() == nullptr, __func__);
    }
    
    void runTests() {
        gameBuilder = new RummyGameBuilder(1);
        emptyGameBuilder();
        notEnoughPlayers();
        validGame();
        tooMuchPlayers();
        resetBuilder();
        delete gameBuilder;
    }

}