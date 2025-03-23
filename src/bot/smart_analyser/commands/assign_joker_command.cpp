#include <bot/smart_analyser/commands/assign_joker_command.h>

using namespace bot;


AssignJokerCommand::AssignJokerCommand(ITileNode* node, ValueDirection direction)
    : _node(node), _originalValue(node->getTileWrapper()->tile->value), _originalColor(node->getTileWrapper()->tile->color), _direction(direction)
{}

void AssignJokerCommand::execute() {
    int newValue = 0;
    Color newColor = _originalColor;
    if (_direction == ValueDirection::Lower) {
        const Tile* frontTile = _node->getMeldTile(MeldDirection::Next)->getTileWrapper()->tile;
        newValue = frontTile->value - 1;
        newColor = frontTile->color;
    }
    else if (_direction == ValueDirection::Higher) {
        const Tile* backTile = _node->getMeldTile(MeldDirection::Previous)->getTileWrapper()->tile;
        newValue = backTile->value + 1;
        newColor = backTile->color;
    }
    else if (_direction == ValueDirection::Same) {
        newValue = _node->getMeldTile(MeldDirection::Previous)->getTileWrapper()->tile->value;
    }
    _node->getTileWrapper()->setJokerValue(newValue, newColor);
}

void AssignJokerCommand::undo() {
    _node->getTileWrapper()->setJokerValue(_originalValue, _originalColor);
}
