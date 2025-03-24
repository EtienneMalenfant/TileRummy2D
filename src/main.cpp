#include <iostream>
#include <thread>

#include <game_builder.h>
#include <game/game_dependencies.h>
#include <gui/game_interface.h>
#include <console_logger.h>
#include <tests/test.h>

int main(int argc, char** argv) {

    #ifdef DEBUG
        test::runTests();
    #endif

    // Créer le jeu
    IGameBuilder* gameBuilder = new RummyGameBuilder(2);

    // set le temps d'attente d'un bot
    if (argc > 1) {
        gameBuilder->setBotWaitTime(std::stoi(argv[1]));
    }
    // mettre une seed si spécifiée
    if (argc > 2) {
        gameBuilder->setStockSeed(std::stoul(argv[2]));
    }

    gameBuilder->addBot("Alice");
    gameBuilder->addBot("Bob");
    gameBuilder->addBot("Charlie");
    gameBuilder->addPlayer("Utilisateur");

    IGame* game = gameBuilder->getGame();
    GameDependencies* dependencies = gameBuilder->getDependencies();

    // Valider les dépendances
    if (game == nullptr || dependencies == nullptr) {
        std::cerr << "Erreur: le jeu n'a pas été correctement initialisé" << std::endl;
        return 1;
    }

    // Abonner les output du jeu
    gui::IGameInterface* gameInterface = new gui::WindowGameInterface(dependencies);
    dependencies->eventPublisher->subscribe(new ConsoleGameEventListener());
    dependencies->eventPublisher->subscribe(gameInterface->getEventListener());

    // Lancer le jeu et la fenêtre
    gameInterface->start();
    std::thread gameThread(&IGame::start, game); // Lancer le jeu sur un autre thread
    while (gameInterface->isRunning()) {
        gameInterface->refresh();
    }
    gameThread.detach(); // tuer le thread si la fenêtre est fermée

    delete gameInterface;
    delete gameBuilder;
    return 0;
}
