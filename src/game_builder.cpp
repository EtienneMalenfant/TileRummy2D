#include <game_builder.h>
#include <game/logger/melds_manager_logger.h>
#include <game/logger/player_controller_logger.h>
#include <game/player_first_meld_handler.h>
#include <bot/smart_analyser/smart_insertions_analyser.h>

RummyGameBuilder::RummyGameBuilder() {
    init();
}

RummyGameBuilder::RummyGameBuilder(int difficultyLevel)
: _difficultyLevel(difficultyLevel)
{
    init();
}

RummyGameBuilder::~RummyGameBuilder() {
    deleteObjects();
}

// ------------------------------
// Privé

void RummyGameBuilder::init() {
    _logger = new FileLogger("last_game.log", true);
    _eventPublisher = new GameEventPublisher();
    MeldsManager* meldsManagerClass = new MeldsManager();
    _meldsContainer = meldsManagerClass;
    _meldsManager = new MeldsManagerLogger(meldsManagerClass, _logger);
    _players = new std::vector<IPlayer*>();
    _stock = new TileStock(_logger);
    _newMeldsAnalyser = new bot::NewMeldsAnalyser();
    initActionsAnalyser();
}

void RummyGameBuilder::initActionsAnalyser() {
    switch (_difficultyLevel) {
        case 1:
            _insertionsAnalyser = new bot::DumbActionsAnalyser(_meldsContainer);
            break;
        case 2:
            _insertionsAnalyser = new bot::SmartInsertionsAnalyser(_meldsContainer);
            break;
        default:
            _insertionsAnalyser = new bot::SmartInsertionsAnalyser(_meldsContainer);
            break;
    }
}

void RummyGameBuilder::deleteObjects() {
    delete _eventPublisher;
    delete _meldsManager;
    delete _stock;
    delete _insertionsAnalyser;
    delete _newMeldsAnalyser;
    delete _logger;
    delete _players;
    delete _guiPlayerController;
    for (int i = 0; i < 4; i++) {
        delete _botPlayer[i];
    }
    delete _game;
    delete _dependencies;
}

void RummyGameBuilder::setNullPointers() {
    _eventPublisher = nullptr;
    _meldsContainer = nullptr;
    _meldsManager = nullptr;
    _guiPlayer = nullptr;
    _players = nullptr;
    _stock = nullptr;
    _insertionsAnalyser = nullptr;
    _newMeldsAnalyser = nullptr;
    _logger = nullptr;
    _guiPlayerController = nullptr;
    for (int i = 0; i < 4; i++) {
        _botPlayer[i] = nullptr;
    }
    _game = nullptr;
    _dependencies = nullptr;
}

bool RummyGameBuilder::dependenciesAreSet() {
    // Vérifier qu'il n'y a rien a nullptr
    return _eventPublisher != nullptr && _meldsContainer != nullptr && _meldsManager != nullptr
        && _insertionsAnalyser != nullptr && _newMeldsAnalyser != nullptr && _players != nullptr;
}

bool RummyGameBuilder::gameIsValid() {
    bool hasRightAmountOfPlayer = _players->size() >= 2 && _players->size() <= 4;
    return dependenciesAreSet() && hasRightAmountOfPlayer;
}

// ------------------------------
// Publique

void RummyGameBuilder::reset() {
    deleteObjects();
    setNullPointers();
    init();
}

void RummyGameBuilder::addBot(const std::string& name) {
    PlayerManager* currentPlayer = new PlayerManager(_stock, _meldsManager, _eventPublisher, name);
    IPlayerController* controller = new PlayerFirstMeldHandler(currentPlayer, currentPlayer);
    controller = new PlayerControllerLogger(controller, _logger);
    // ajout dans la liste des joueurs
    _players->push_back(currentPlayer);

    bot::BotPlayer* botPlayer = new bot::BotPlayer(controller, currentPlayer, _insertionsAnalyser, _newMeldsAnalyser);
    // garger le IBotPlayer
    _botPlayer[_players->size()] = botPlayer;
    botPlayer->setWaitTime(_waitTime);
    // pour recevoir les event de tours
    _eventPublisher->subscribe(botPlayer);

    // S'il n'y a pas de joueur utilisateur
    if (_guiPlayer == nullptr && _guiPlayerController == nullptr) {
        // Mettre un bot en tant que joueur principal
        _guiPlayer = currentPlayer;
    }

}

void RummyGameBuilder::addPlayer(const std::string& name) {
    PlayerManager* currentPlayer = new PlayerManager(_stock, _meldsManager, _eventPublisher, name);
    _guiPlayer = currentPlayer;
    _guiPlayerController = new PlayerFirstMeldHandler(currentPlayer, currentPlayer);
    _players->push_back(_guiPlayer);
}

void RummyGameBuilder::setStockSeed(unsigned int seed) {
    _stock->setSeed(seed);
}

void RummyGameBuilder::setBotWaitTime(unsigned int time) {
    _waitTime = time;
    for (int i = 0; i < 4; i++) {
        if (_botPlayer[i] != nullptr) {
            _botPlayer[i]->setWaitTime(time);
        }
    }
}

// ------------------------------

IGame* RummyGameBuilder::getGame() {
    if (gameIsValid()) {
        if (_game == nullptr) {
            RummyGame* game = new RummyGame(_players, _eventPublisher);
            _game = game;
            _eventPublisher->subscribe(game);
        }
        return _game;
    }
    return nullptr;
}

GameDependencies* RummyGameBuilder::getDependencies() {
    if (gameIsValid()) {
        if (_dependencies == nullptr) {
            _dependencies = new GameDependencies(_eventPublisher, _meldsContainer, _guiPlayerController, _guiPlayer, _players);
        }
        return _dependencies;
    }
    return nullptr;
}
