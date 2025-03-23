#pragma once
#include <i_command.h>
#include <bot/smart_analyser/tile_node.h>

namespace bot {

    class AssignJokerCommand : public ICommand {
    private:
        const int _originalValue;
        const Color _originalColor;
        ITileNode* _node;

        ValueDirection _direction;
    public:
        AssignJokerCommand(ITileNode* node, ValueDirection direction);
        void execute() override;
        void undo() override;
    };
}
