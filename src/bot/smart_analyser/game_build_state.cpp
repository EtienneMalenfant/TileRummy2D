#include <bot/smart_analyser/game_build_state.h>

using namespace bot;

GameBuildState::~GameBuildState() {
    for (ITileGroup* group : _groups) {
        delete group;
    }
}

bool GameBuildState::contains(ITileNode* tileNode) {
    for (ITileGroup* group : _groups) {
        if (group->contains(tileNode)) {
            return true;
        }
    }
    return false;
}

int GameBuildState::getPlayerTileCount() {
    int count = 0;
    for (ITileGroup* group : _groups) {
        count += group->getPlayerTileCount();
    }
    return count;
}

void GameBuildState::add(ITileGroup* group) {
    _groups.push_back(group);
}

std::list<std::list<Action*>*>* GameBuildState::toActions() {
    std::list<std::list<Action*>*>* actionsList = new std::list<std::list<Action*>*>();
    for (ITileGroup* group : _groups) {
        actionsList->push_back(group->toActions());
    }
    return actionsList;
}
