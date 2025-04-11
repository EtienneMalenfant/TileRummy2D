#include <bot/smart_analyser/commands/add_node_command.h>
#include <bot/smart_analyser/commands/link_node_command.h>
#include <bot/smart_analyser/commands/assign_joker_command.h>

using namespace bot;

AddNodeCommand::AddNodeCommand(ITileNode* node, std::vector<ITileNode*>* meld, ValueDirection direction) {
    _linkNodeCommand = new LinkNodeCommand(node, meld, direction);
    if (node->getTileWrapper()->tile->isJoker) {
        _assignJokerCommand = new AssignJokerCommand(node, direction);
    }
}

AddNodeCommand::~AddNodeCommand() {
    delete _linkNodeCommand;
    delete _assignJokerCommand;
}

void AddNodeCommand::execute() {
    // l'ordre d'execution est important
    _linkNodeCommand->execute();
    if (_assignJokerCommand != nullptr) {
        _assignJokerCommand->execute();
    }
}

void AddNodeCommand::undo() {
    if (_assignJokerCommand != nullptr) {
        _assignJokerCommand->undo();
    }
    _linkNodeCommand->undo();
}
